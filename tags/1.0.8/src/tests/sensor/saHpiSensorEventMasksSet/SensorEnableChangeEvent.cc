/*
 * (C) Copyright University of New Hampshire, 2006
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author(s):
 *     Donald A. Barre <dbarre@unh.edu>
 */

#include "SensorEnableChangeEvent.h"
#include "ListIterator.h"
#include "EventMatcher.h"
#include "EventHelper.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorEnableChangeEvent::SensorEnableChangeEvent(char *line)
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SensorEnableChangeEvent::getName() {
    return "SensorEnableChangeEvent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SensorEnableChangeEvent::getDescription() {
    return "If a sensor's assert and/or deassert event mask changes as a\n"
           "result of a call to saHpiSensorEventMasksSet(), a Sensor Enable\n"
           "Change Event will be generated.  The assert and deassert event\n"
           "masks will be independently changed.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SensorEnableChangeEvent::getPrecondition() {
    return "To perform this test, one or more sensors must be found\n"
           "whose assert and deassert event masks can be independently\n"
           "changed.  Thus, the resource's <i>ResourceCapabilities</i>\n"
           "cannot have the SAHPI_CAPABILITY_EVT_DEASSERTS capability set.";
}

/*****************************************************************************
 * Run the test.
 *
 * Process each resource in the domain.  For each resource, its sensors will
 * be tested.  Since events are asynchronous, we will have to wait for events
 * after finishing with the resources.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        sensorCount = 0;
        foundEventCount = 0;

        // Test each resource.

        SaHpiRptEntryT rptEntry;
        SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
        while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY) {
            SaHpiEntryIdT entryId = nextEntryId;
            error = saHpiRptEntryGet(sessionId, entryId,
                                     &nextEntryId, &rptEntry);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
                break;
            } else if (hasSensorCapability(&rptEntry) && 
                       !hasEvtDeassertsCapability(&rptEntry)) {
                HpiTestStatus testStatus = testResource(sessionId, rptEntry.ResourceId);
                if (testStatus.hasFault()) {
                    status.add(TRACE, testStatus);
                }
            }
        }

        // If we didn't encounter any faults, and if we didn't test any
        // sensors, then set a status of Not Supported. Otherwise, poll
        // for any remaining events that are still expected to arrive.

        if (!status.hasFault()) {
            if (sensorCount == 0) {
                status.assertNotSupport();
            } else {
                // Check for any remaining events that may have been generated.
                // We will poll for up to "n" seconds to give the last sensor time to
                // generate an event.

                status.add(TRACE, pollEvents(sessionId, getEventDelayTime() * 4));

                if (status.isOkay()) {
                    status.add(TRACE, verifyEvents());
                }
            }
        }

        status.add(TRACE, restoreSensors(sessionId));

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Test a Resource.
 *
 * For each RDR in the RDR table, if it a sensor that we can test, then do so.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::testResource(SaHpiSessionIdT sessionId,
                                                    SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiRdrT rdr;

    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRdrGet(sessionId, resourceId,
                                     entryId, &nextEntryId, &rdr);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RDR_GET, SA_OK, error);
            break;
        } else if (canTestRdr(&rdr)) {
            SaHpiSensorRecT *sensorRec = &(rdr.RdrTypeUnion.SensorRec);
            status.add(TRACE, testSensor(sessionId, resourceId, sensorRec));
        }
    }

    return status;
}

/*****************************************************************************
 * Can we test this RDR?  Must meet the following requirements:
 *
 * 1. This is a sensor.
 * 2. The sensor's event masks can be changed.
 * 3. The sensor has event states that can be asserted/deasserted.
 * 4. We haven't reached the maximum number of testable sensors.
 *****************************************************************************/

bool SensorEnableChangeEvent::canTestRdr(SaHpiRdrT *rdr) {
    if ((rdr->RdrType == SAHPI_SENSOR_RDR) &&
        (sensorCount < MAX_THRESHOLD_SENSORS) &&
        (rdr->RdrTypeUnion.SensorRec.EventCtrl == SAHPI_SEC_PER_EVENT) &&
        (rdr->RdrTypeUnion.SensorRec.Events != 0x0)) {

        return true;
    }

    return false;
}

/*****************************************************************************
 * Test a sensor by changing the event masks. Depending upon the current
 * asserted event states, add or remove event states.  Save the sensor data so
 * that we can correlate it with the incoming events. 
 *
 * Since we want to add and remove event states independently to the assert 
 * and deassert event masks, we set use a set of states indicating what action
 * to perform next.  As events arrive in the handleEvent() method, we move
 * to the next state and perform the necessary action to generate a new
 * event.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::testSensor(SaHpiSessionIdT sessionId,
                                                  SaHpiResourceIdT resourceId,
                                                  SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    pushLocation("Resource", resourceId);
    pushLocation("Sensor", sensorRec->Num);

    SaErrorT error = saHpiSensorEventMasksGet(sessionId, resourceId, sensorRec->Num,
                                              &assertEventMask, &deassertEventMask);
    if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
    } else {
        sensorData[sensorCount].resourceId = resourceId;
        sensorData[sensorCount].sensorNum = sensorRec->Num;
        sensorData[sensorCount].origAssertEventMask = assertEventMask;
        sensorData[sensorCount].origDeassertEventMask = deassertEventMask;
        sensorData[sensorCount].stateIndex = 0;
        if (assertEventMask == 0) {
            sensorData[sensorCount].state[0] = STATE_ADD_ASSERT;
            sensorData[sensorCount].state[1] = STATE_REMOVE_ASSERT;
            status.add(TRACE, changeEventMasks(sessionId, resourceId, sensorRec->Num, 
                                               SAHPI_SENS_ADD_EVENTS_TO_MASKS, 
                                               SAHPI_ALL_EVENT_STATES, 0x0));
        } else {
            sensorData[sensorCount].state[0] = STATE_REMOVE_ASSERT;
            sensorData[sensorCount].state[1] = STATE_ADD_ASSERT;
            status.add(TRACE, changeEventMasks(sessionId, resourceId, sensorRec->Num, 
                                               SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS, 
                                               SAHPI_ALL_EVENT_STATES, 0x0));
        }

        if (deassertEventMask == 0) {
            sensorData[sensorCount].state[2] = STATE_ADD_DEASSERT;
            sensorData[sensorCount].state[3] = STATE_REMOVE_DEASSERT;
        } else {
            sensorData[sensorCount].state[2] = STATE_REMOVE_DEASSERT;
            sensorData[sensorCount].state[3] = STATE_ADD_DEASSERT;
        }

        sensorCount++;
    }

    popLocation();
    popLocation();

    if (status.isOkay() && (sensorCount % POLL_COUNT == 0)) {
        status.add(TRACE, pollEvents(sessionId, -1));
    }

    return status;
}

/*****************************************************************************
 * Poll for events.
 * 
 * The "timeout" is in milliseconds.  If it is -1, then the timeout is 
 * immediate.  The Poller will look for Sensor Enable Change Events.  When one 
 * is found, the below handleEvent() method will be invoked.
 * 
 * Note that we minimize the amount of polling we do, so we will only
 * poll when we've added POLL_COUNT sensors.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::pollEvents(SaHpiSessionIdT sessionId,
                                                  long timeout) {
    HpiTestStatus status;

    EventMatcher eventMatcher;
    eventMatcher.setEventType(SAHPI_ET_SENSOR_ENABLE_CHANGE);
    status.add(TRACE, EventHelper::pollEvents(sessionId, timeout,
                                              eventMatcher, this));

    return status;
}

/*****************************************************************************
 * If the Poller encounters a Sensor Enable Change Event, this handleEvent() 
 * method will be invoked.  When invoked, we will search the Sensor Data for 
 * the corresponding sensor.  If found, we look at what the next action is.
 * Keep in mind that we need to get two events: one for adding event states
 * and one for removing event states from the event masks.  After receiving
 * the first event, then add or remove event states.  
 *
 * The "foundEventCount" field is used to determine if we have found all of 
 * the expected events for all of the sensors.  
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::handleEvent(SaHpiSessionIdT sessionId,
                                                   SaHpiEventT *event) {
    HpiTestStatus status;

    SaHpiSensorNumT sensorNum = event->EventDataUnion.SensorEnableChangeEvent.SensorNum;

    for (int i = 0; i < sensorCount; i++) {
        if ((sensorData[i].resourceId == event->Source) && 
            (sensorData[i].sensorNum == sensorNum)) {

            pushLocation("Resource", event->Source);
            pushLocation("Sensor", sensorNum);

            sensorData[i].stateIndex++;
            int index = sensorData[i].stateIndex;
            if (index < 4) {
                if (sensorData[i].state[index] == STATE_ADD_ASSERT) {
                    status.add(TRACE, changeEventMasks(sessionId, sensorData[i].resourceId,
                                                       sensorData[i].sensorNum,
                                                       SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                       SAHPI_ALL_EVENT_STATES, 0x0));
                } else if (sensorData[i].state[index] == STATE_REMOVE_ASSERT) {
                    status.add(TRACE, changeEventMasks(sessionId, sensorData[i].resourceId,
                                                       sensorData[i].sensorNum,
                                                       SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS,
                                                       SAHPI_ALL_EVENT_STATES, 0x0));
                } else if (sensorData[i].state[index] == STATE_ADD_DEASSERT) {
                    status.add(TRACE, changeEventMasks(sessionId, sensorData[i].resourceId,
                                                       sensorData[i].sensorNum,
                                                       SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                       0x0, SAHPI_ALL_EVENT_STATES));
                } else if (sensorData[i].state[index] == STATE_REMOVE_DEASSERT) {
                    status.add(TRACE, changeEventMasks(sessionId, sensorData[i].resourceId,
                                                       sensorData[i].sensorNum,
                                                       SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS,
                                                       0x0, SAHPI_ALL_EVENT_STATES));
                }
            }

            foundEventCount++;

            popLocation();
            popLocation();

            break;
        }
    }

    return status;
}

/*****************************************************************************
 * This method is used to prematurely stop polling.  The Poller has a timeout
 * period that must expire before exiting.  But if we have found all of the
 * events for all of the sensors, then there is no need to continue waiting.
 * Thus, we can instruct the Poller to exit early without waiting for the
 * entire timeout period.  In this test, there are four events per sensor.
 *****************************************************************************/

bool SensorEnableChangeEvent::isEventProcessingDone() {
    return (foundEventCount == sensorCount * 4);
}

/*****************************************************************************
 * Check all of the sensors that were tested.  We should have recieved a
 * four sensor enable change events for each.  If not, then record a failure 
 * for each of those sensors.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::verifyEvents() {
    HpiTestStatus status;

    status.assertPass();
    for (int i = 0; i < sensorCount; i++) {
        if (sensorData[i].stateIndex < 4) {
            pushLocation("Resource", sensorData[i].resourceId);
            pushLocation("Sensor", sensorData[i].sensorNum);
            status.assertFailure(TRACE, "Did not receive a Sensor Enable Change Event.");
            popLocation();
            popLocation();
        }
    }

    return status;
}

/*****************************************************************************
 * Restore the original event masks for each the sensor.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::restoreSensors(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    for (int i = 0; i < sensorCount; i++) {
        status.add(TRACE, resetEventMasks(sessionId,
                                          sensorData[i].resourceId,
                                          sensorData[i].sensorNum, 
                                          sensorData[i].origAssertEventMask,
                                          sensorData[i].origAssertEventMask));
    }

    return status;
}

/*****************************************************************************
 * Change the event mask by either adding all of the supported event 
 * states or by removing all of the event states.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::changeEventMasks(SaHpiSessionIdT sessionId,
                                                        SaHpiResourceIdT resourceId,
                                                        SaHpiSensorNumT sensorNum,
                                                        SaHpiSensorEventMaskActionT action,
                                                        SaHpiEventStateT assertEventMask,
                                                        SaHpiEventStateT deassertEventMask) {

    HpiTestStatus status;

    SaErrorT error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum, action,
                                              assertEventMask, deassertEventMask);
    if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
    }

    return status;
}

/*****************************************************************************
 * Reset the event masks for the given sensor.  This is done by clearing
 * all of the event state bits and then adding in the event state bits.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::resetEventMasks(SaHpiSessionIdT sessionId,
                                                       SaHpiResourceIdT resourceId,
                                                       SaHpiSensorNumT sensorNum,
                                                       SaHpiEventStateT assertEventMask, 
                                                       SaHpiEventStateT deassertEventMask) {

    HpiTestStatus status;

    // clear all of the bits
    SaErrorT error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum,
                                              SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS,
                                              SAHPI_ALL_EVENT_STATES, SAHPI_ALL_EVENT_STATES);
    if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
    } else {
        error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum,
                                         SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                         assertEventMask, deassertEventMask);

        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
        }
    }

    return status;
}


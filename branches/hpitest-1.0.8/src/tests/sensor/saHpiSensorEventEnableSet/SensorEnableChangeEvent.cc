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

using namespace ns_saHpiSensorEventEnableSet;

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
    return "If a sensor's event enable status changes as a result of a call\n"
           "to saHpiSensorEventEnableSet(), a Sensor Enable Change Event\n"
           "will be generated.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SensorEnableChangeEvent::getPrecondition() {
    return "To perform this test, one or more sensors must be found\n"
           "whose event enable status can be changed.";
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

        // test each resource

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
            } else if (hasSensorCapability(&rptEntry)) {
                HpiTestStatus testStatus = testResource(sessionId,
                                                        rptEntry.ResourceId);
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

                status.add(TRACE, pollEvents(sessionId, getEventDelayTime() * 2));

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
 * 2. The sensor's event enable status can be changed.
 * 3. We haven't reached the maximum number of testable sensors.
 *****************************************************************************/

bool SensorEnableChangeEvent::canTestRdr(SaHpiRdrT *rdr) {
    if (rdr->RdrType == SAHPI_SENSOR_RDR) {
        if ((sensorCount < MAX_THRESHOLD_SENSORS) &&
            (rdr->RdrTypeUnion.SensorRec.EventCtrl != SAHPI_SEC_READ_ONLY)) {
            return true;
        }
    }

    return false;
}

/*****************************************************************************
 * Test a sensor by toggling its event enable status.  Save the sensor data so
 * that we can coorolate it the incoming events.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::testSensor(SaHpiSessionIdT sessionId,
                                                  SaHpiResourceIdT resourceId,
                                                  SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiBoolT sensorEventEnabled;

    pushLocation("Resource", resourceId);
    pushLocation("Sensor", sensorRec->Num);

    SaErrorT error = saHpiSensorEventEnableGet(sessionId, resourceId,
                                               sensorRec->Num, &sensorEventEnabled);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_ENABLE_GET, SA_OK, error);
    } else {
        error = saHpiSensorEventEnableSet(sessionId, resourceId,
                                          sensorRec->Num, !sensorEventEnabled);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_ENABLE_SET, SA_OK, error);
        } else {
            sensorData[sensorCount].resourceId = resourceId;
            sensorData[sensorCount].sensorNum = sensorRec->Num;
            sensorData[sensorCount].enabled = sensorEventEnabled;
            sensorData[sensorCount].foundFirstEvent = false;
            sensorData[sensorCount].foundSecondEvent = false;
            sensorCount++;
        }
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
 * the corresponding sensor.  If found, we mark that sensor's "foundEvent" 
 * field which indicates that it worked properly.  The "foundEventCount" field is
 * used to determine if we have found all of the expected events for all of
 * the sensors.  
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::handleEvent(SaHpiSessionIdT sessionId,
                                                   SaHpiEventT *event) {
    HpiTestStatus status;

    SaHpiSensorNumT sensorNum = event->EventDataUnion.SensorEnableChangeEvent.SensorNum;
    SaHpiSensorNumT enabled = event->EventDataUnion.SensorEnableChangeEvent.SensorEventEnable;

    for (int i = 0; i < sensorCount; i++) {
        if ((sensorData[i].resourceId == event->Source) && 
            (sensorData[i].sensorNum == sensorNum)) {

            pushLocation("Resource", event->Source);
            pushLocation("Sensor", sensorNum);

            if (!sensorData[i].foundFirstEvent) {

                // The enabled status must be the opposite of the original state.

                if (!isBoolEqual(sensorData[i].enabled, enabled)) {
                    foundEventCount++;
                    sensorData[i].foundFirstEvent = true;
                    SaErrorT error = saHpiSensorEventEnableSet(sessionId,
                                                               sensorData[i].resourceId,
                                                               sensorData[i].sensorNum,
                                                               sensorData[i].enabled);
                    status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
                }
            } else if ((!sensorData[i].foundSecondEvent) && 
                       (isBoolEqual(sensorData[i].enabled, enabled))) {
                foundEventCount++;
                sensorData[i].foundSecondEvent = true;
            }

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
 * entire timeout period.  In this case, we expect two events for each
 * sensor.
 *****************************************************************************/

bool SensorEnableChangeEvent::isEventProcessingDone() {
    return (foundEventCount == sensorCount * 2);
}

/*****************************************************************************
 * Check all of the sensors that were tested.  We should have recieved two
 * sensor enable change events for each.  If not, then record a failure for
 * each of those sensors.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::verifyEvents() {
    HpiTestStatus status;

    status.assertPass();
    for (int i = 0; i < sensorCount; i++) {
        if (!sensorData[i].foundFirstEvent) {
            pushLocation("Resource", sensorData[i].resourceId);
            pushLocation("Sensor", sensorData[i].sensorNum);
            status.assertFailure(TRACE, "Did not get a Sensor Enable Change Event.\n"
                              "The event enable state was set to %s.", 
                              HpiString::boolean(!sensorData[i].enabled));
            popLocation();
            popLocation();
        } else if (!sensorData[i].foundSecondEvent) {
            pushLocation("Resource", sensorData[i].resourceId);
            pushLocation("Sensor", sensorData[i].sensorNum);
            status.assertFailure(TRACE, "Did not get a Sensor Enable Change Event.\n"
                              "The event enable state was set to %s.", 
                              HpiString::boolean(sensorData[i].enabled));
            popLocation();
            popLocation();
        }
    }

    return status;
}

/*****************************************************************************
 * Restore the original event enable state for each sensor.
 *****************************************************************************/

HpiTestStatus SensorEnableChangeEvent::restoreSensors(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    for (int i = 0; i < sensorCount; i++) {
        SaErrorT error = saHpiSensorEventEnableSet(sessionId, 
                                                   sensorData[i].resourceId,
                                                   sensorData[i].sensorNum,
                                                   sensorData[i].enabled);
        status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
    }

    return status;
}


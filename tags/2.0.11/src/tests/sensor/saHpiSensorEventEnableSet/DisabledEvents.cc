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

#include "DisabledEvents.h"
#include "ListIterator.h"
#include "EventMatcher.h"
#include "EventHelper.h"

using namespace ns_saHpiSensorEventEnableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DisabledEvents::DisabledEvents(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DisabledEvents::getName() {
    return "DisabledEvents";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DisabledEvents::getDescription() {
    return "If event generation for a sensor is disabled, no events will be\n"
           "generated as a result of the assertion or deassertion of any event\n"
           "state, regardless of the setting of the assert or deassert event\n"
           "masks for the sensor.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DisabledEvents::getPrecondition() {
    return "To perform this test, one or more threshold sensors must be found \n"
           "that meet the following conditions:\n"
           "<ol>"
           "<li>The sensor is enabled or can be enabled.</li>\n"
           "<li>The sensor's event status can be disabled.</li>\n"
           "<li>The sensor can generate an UPPER MINOR event.</li>\n"
           "<li>The sensor's AssertEventMask is set or can be set for an "
                   "UPPER MINOR event.</li>\n"
           "<li>The sensor's LowMinor and UpMinor thresholds are supported.</li>\n"
           "<li>The sensor's reading is normal, i.e. between the LowMinor "
                   "and UpMinor threshold values.</li>\n"
           "<li>The LowMinor and UpMinor threshold values are readable.</li>\n"
           "<li>The UpMinor threshold value is writeable.</li>"
           "</ol>";
}

/*****************************************************************************
 * Run the test.
 *
 * Process each resource in the domain.  For each resource, its sensors will
 * be tested.  Since events are asynchronous, we will have to wait for events
 * after finishing with the resources.
 *****************************************************************************/

HpiTestStatus DisabledEvents::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        sensorCount = 0;

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

        // If we didn't encounter any faults, we first determine if
        // we actually tested any sensors.  If not, then return Not Supported.
        // Otherwise, we need to finish polling for any more events that
        // haven't come in yet.

        if (!status.hasFault()) {
            if (sensorCount == 0) {
                status.assertNotSupport();
            } else {
                status.assertPass();

                // Check for any remaining events that may have been generated.
                // We will poll for up to "n" seconds to give the last sensor time to
                // generate an event.
    
                status.add(TRACE, pollEvents(sessionId, getEventDelayTime()));
            }
        }

        status.add(TRACE, restoreSensors());

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Test a Resource.
 *
 * For each RDR entry, determine if it is a threshold sensor that we can 
 * test.  If so, then test that sensor.
 *****************************************************************************/

HpiTestStatus DisabledEvents::testResource(SaHpiSessionIdT sessionId,
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
 * Can we test this RDR?  It must meet the following requirements:
 *
 * 1. It must be a threshold sensor.
 * 2. We have to be able to change the sensor's event enable status.
 * 3. We can't exceed the number of sensors we can test.
 *****************************************************************************/

bool DisabledEvents::canTestRdr(SaHpiRdrT *rdr) {
    if (rdr->RdrType == SAHPI_SENSOR_RDR) {
        SaHpiSensorRecT *sensorRec = &(rdr->RdrTypeUnion.SensorRec);
        if ((sensorCount < MAX_THRESHOLD_SENSORS) &&
            (sensorRec->ThresholdDefn.IsAccessible) &&
            (sensorRec->EventCtrl != SAHPI_SEC_READ_ONLY)) {

            return true;
        }
    }
    return false;
}

/*****************************************************************************
 * Test a sensor.
 *
 * Prepare to generate an UpMinor Assertion Event.  If we determine that we
 * can generate the event, then we do so and we record the event generator.  
 * The event generator is recorded so that it can be retrieved later when
 * the event is received.
 *****************************************************************************/

HpiTestStatus DisabledEvents::testSensor(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT resourceId,
                                         SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    bool canAssert;

    pushLocation("Resource", resourceId);
    pushLocation("Sensor", sensorRec->Num);

    UpMinorEventGenerator *gen = new UpMinorEventGenerator(sessionId, resourceId, sensorRec);
    HpiTestStatus genStatus = gen->canAssertEvent(&canAssert);
    if (!genStatus.isOkay()) {
        status.add(TRACE, genStatus);
        status.add(TRACE, gen->restoreSensor());
        delete gen;
    } else if (!canAssert) {
        status.add(TRACE, gen->restoreSensor());
        delete gen;
    } else {
        gen->setDisableEvents();
        status.add(TRACE, gen->assertEvent());
        if (status.isOkay()) {
            sensorData[sensorCount].upMinorEventGenerator = gen;
            sensorCount++;
        } else {
            status.add(TRACE, gen->restoreSensor());
            delete gen;
        }
    }

    popLocation();
    popLocation();

    // Periodically poll for events so that the event queue doesn't overflow.

    if (status.isOkay() && (sensorCount % POLL_COUNT == 0)) {
        status.add(TRACE, pollEvents(sessionId, -1));
    }

    return status;
}

/*****************************************************************************
 * Poll for events.
 * 
 * The "timeout" is in milliseconds.  If it is -1, then the timeout is 
 * immediate.  The Poller will look for Sensor Events.  When one is found,
 * the below handleEvent() method will be invoked.
 * 
 * Note that we minimize the amount of polling we do, so we will only
 * poll when we've added POLL_COUNT sensors.
 *****************************************************************************/

HpiTestStatus DisabledEvents::pollEvents(SaHpiSessionIdT sessionId, long timeout) {
    HpiTestStatus status;

    EventMatcher eventMatcher;
    eventMatcher.setEventType(SAHPI_ET_SENSOR);
    eventMatcher.setSensorEventState(SAHPI_ES_UPPER_MINOR);
    status.add(TRACE, EventHelper::pollEvents(sessionId, timeout,
                                              eventMatcher, this));

    return status;
}

/*****************************************************************************
 * If the Poller encounters a Sensor Event, this handleEvent() method will
 * be invoked.  When invoked, we will search the Sensor Data for the
 * UpMinor Event Generator that matches the event.  Since sensor events are 
 * disabled, we should never find an event that matches.  If we do, we can
 * return a failure.
 *****************************************************************************/

HpiTestStatus DisabledEvents::handleEvent(SaHpiSessionIdT ,
                                          SaHpiEventT *event) {
    HpiTestStatus status;

    if (event->EventDataUnion.SensorEvent.Assertion) {
        SaHpiSensorNumT sensorNum = event->EventDataUnion.SensorEvent.SensorNum;
        for (int i = 0; i < sensorCount; i++) {
            if (sensorData[i].upMinorEventGenerator->matches(event->Source, sensorNum)) {
                pushLocation("Resource", event->Source);
                pushLocation("Sensor", sensorNum);
                status.assertFailure(TRACE, "Sensor with disabled events generated an event!");
                popLocation();
                popLocation();
                break;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * The isEventProcessingDone() method is used to prematurely stop polling
 * for events.  In this test, always return false, i.e. always poll for the
 * given timeout period.
 *****************************************************************************/

bool DisabledEvents::isEventProcessingDone() {
    return false;
}

/*****************************************************************************
 * Restore the sensors to their original settings.
 *****************************************************************************/

HpiTestStatus DisabledEvents::restoreSensors() {
    HpiTestStatus status;

    for (int i = 0; i < sensorCount; i++) {
        status.add(TRACE, sensorData[i].upMinorEventGenerator->restoreSensor());
        delete sensorData[i].upMinorEventGenerator;
    }

    return status;
}


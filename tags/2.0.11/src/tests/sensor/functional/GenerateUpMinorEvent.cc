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

#include "GenerateUpMinorEvent.h"
#include "ListIterator.h"
#include "EventMatcher.h"
#include "EventHelper.h"

using namespace ns_saHpiSensorFunctional;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GenerateUpMinorEvent::GenerateUpMinorEvent(char *line)
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GenerateUpMinorEvent::getName() {
    return "GenerateUpMinorEvent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GenerateUpMinorEvent::getDescription() {
    return "Generate an UpMinor event.  If the sensor UpMinor assertion\n"
           "event is successfully receieved, the event will then be\n"
           "deasserted.  The test case will verify that the deassert event\n"
           "is generated.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GenerateUpMinorEvent::getPrecondition() {
    return "To perform this test, one or more threshold sensors must be\n"
           "found that meet the following conditions:\n"
           "<ol>\n"
           "<li>The sensor that is enabled or can be enabled.</li>\n"
           "<li>The sensor's event status is enabled or can be enabled.</li>\n"
           "<li>The sensor can generate an UPPER MINOR event.</li>\n"
           "<li>The sensor's AssertEventMask is set or can be set for "
               "an UPPER MINOR event.</li>\n"
           "<li>The sensor's LowMinor and UpMinor thresholds are supported.</li>\n"
           "<li>The sensor's reading is normal, i.e. between the LowMinor "
               "and UpMinor threshold values.</li>\n"
           "<li>The LowMinor and UpMinor threshold values are readable.</li>\n"
           "<li>The UpMinor threshold value is writeable.</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Run the test.
 *
 * Process each resource in the domain.  For each resource, its sensors will
 * be tested.  Since events are asynchronous, we will have to wait for events
 * after finishing with the resources.
 *****************************************************************************/

HpiTestStatus GenerateUpMinorEvent::runSessionTest(SaHpiSessionIdT sessionId) {
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
            } else if (hasSensorCapability(&rptEntry)) {
                HpiTestStatus testStatus = testResource(sessionId, rptEntry.ResourceId);
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
                // Check for any remaining events that may have been generated.
                // We will poll for up to "n" seconds to give the last sensor time to
                // generate an event.
    
                status.add(TRACE, pollEvents(sessionId, getEventDelayTime()));
                if (status.isOkay()) {
                    status.add(TRACE, verifyEvents());
                }
            }
        }

        status.add(TRACE, restoreSensors());

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Test a resource.
 *
 * For each RDR entry, determine if it is a threshold sensor that we can 
 * test.  If so, then test that sensor.
 *****************************************************************************/

HpiTestStatus GenerateUpMinorEvent::testResource(SaHpiSessionIdT sessionId,
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
 * 1. It must be a Threshold sensor.
 * 2. We can't exceed the number of sensors we can test.
 *****************************************************************************/

bool GenerateUpMinorEvent::canTestRdr(SaHpiRdrT *rdr) {
    if (rdr->RdrType == SAHPI_SENSOR_RDR) {
        SaHpiSensorRecT *sensorRec = &(rdr->RdrTypeUnion.SensorRec);
        if ((sensorCount < MAX_THRESHOLD_SENSORS) &&
            (sensorRec->ThresholdDefn.IsAccessible)) {

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

HpiTestStatus GenerateUpMinorEvent::testSensor(SaHpiSessionIdT sessionId,
                                               SaHpiResourceIdT resourceId,
                                               SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    bool canAssert;

    pushLocation("Resource", resourceId);
    pushLocation("Sensor", sensorRec->Num);

    UpMinorEventGenerator *gen = 
                new UpMinorEventGenerator(sessionId, resourceId, sensorRec);
    HpiTestStatus genStatus = gen->canAssertEvent(&canAssert);
    if (!genStatus.isOkay()) {
        status.add(TRACE, genStatus);
        status.add(TRACE, gen->restoreSensor());
        delete gen;
    } else if (!canAssert) {
        status.add(TRACE, gen->restoreSensor());
        delete gen;
    } else {
        status.add(TRACE, gen->assertEvent());
        if (!status.isOkay()) {
            status.add(TRACE, gen->restoreSensor());
            delete gen;
        } else { 
            sensorData[sensorCount].upMinorEventGenerator = gen;
            sensorData[sensorCount].foundAssertEvent = false;
            sensorData[sensorCount].foundDeassertEvent = false;
            sensorCount++;
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

HpiTestStatus GenerateUpMinorEvent::pollEvents(SaHpiSessionIdT sessionId,
                                               long timeout) {
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
 * UpMinor Event Generator that matches the event.  If we find the sensor then
 * mark it so that we know it's UpMinor event was found.
 *****************************************************************************/

HpiTestStatus GenerateUpMinorEvent::handleEvent(SaHpiSessionIdT ,
                                                SaHpiEventT *event) {
    HpiTestStatus status;

    SaHpiSensorEventT *sensorEvent = &(event->EventDataUnion.SensorEvent);
    SaHpiSensorNumT sensorNum = sensorEvent->SensorNum;
    for (int i = 0; i < sensorCount; i++) {
        if (sensorData[i].upMinorEventGenerator->matches(event->Source, sensorNum)) {
            foundEventCount++;
            pushLocation("Resource", event->Source);
            pushLocation("Sensor", sensorNum);
            if (sensorEvent->Assertion) {
                if (sensorData[i].foundAssertEvent) {
                    status.assertFailure(TRACE, 
                            "Received two sensor UpMinor assert events.");
                } else {
                    sensorData[i].foundAssertEvent = true;
                    status.add(TRACE, 
                            sensorData[i].upMinorEventGenerator->deassertEvent());
                }
            } else {
                if (sensorData[i].foundDeassertEvent) {
                    status.assertFailure(TRACE, 
                            "Received two sensor UpMinor deassert events.");
                } else {
                    sensorData[i].foundDeassertEvent = true;
                }
            }
            popLocation();
            popLocation();
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * The isEventProcessingDone() method is used to prematurely stop polling
 * for events.  In this test, we don't need to continue polling the event
 * queue once we have found both events for each sensor.
 *****************************************************************************/

bool GenerateUpMinorEvent::isEventProcessingDone() {
    return (foundEventCount == sensorCount * 2);
}

/*****************************************************************************
 * Verify that we received all of the UpMinor events we expected.  If we
 * didn't get an event, report that as a failure.
 *****************************************************************************/

HpiTestStatus GenerateUpMinorEvent::verifyEvents() {
    HpiTestStatus status;

    status.assertPass();
    for (int i = 0; i < sensorCount; i++) {

        pushLocation("Resource", 
                    sensorData[i].upMinorEventGenerator->getResourceId());
        pushLocation("Sensor", 
                    sensorData[i].upMinorEventGenerator->getSensorNum());

        if (!sensorData[i].foundAssertEvent) {
            status.assertFailure(TRACE, "Did not receive an UpMinor assert event!");
        } else if (!sensorData[i].foundDeassertEvent) {
            status.assertFailure(TRACE, "Did not receive an UpMinor deassert event!");
        }

        popLocation();
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Restore the sensors to the original settings.
 *****************************************************************************/

HpiTestStatus GenerateUpMinorEvent::restoreSensors() {
    HpiTestStatus status;

    for (int i = 0; i < sensorCount; i++) {
        status.add(TRACE, sensorData[i].upMinorEventGenerator->restoreSensor());
        delete sensorData[i].upMinorEventGenerator;
    }

    return status;
}


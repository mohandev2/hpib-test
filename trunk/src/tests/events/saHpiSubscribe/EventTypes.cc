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

#include "EventTypes.h"
#include "EventHelper.h"

using namespace ns_saHpiSubscribe;

/*****************************************************************************
 * Constants
 *****************************************************************************/

// one second (nanosec)
#define TIMEOUT 1000000000ll

// 10 seconds (msec)
#define MAX_DURATION 10000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EventTypes::EventTypes(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EventTypes::getName() {
    return "EventTypes";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *EventTypes::getDescription() {
    return "Subscription allows the HPI User to get events of all event types.\n"
           "Generate a sensor enable change event in order to verify that an event\n"
           "with an event type of SAHPI_ET_SENSOR_ENABLE_CHANGE is received.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *EventTypes::getPrecondition() {
    return "Requires a sensor that can generate a Sensor Enable Change event.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EventTypes::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus EventTypes::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventT userEvent;
    bool foundUserEvent;
    bool foundSensorEvent;
    EventMatcher matcher;

    foundUserEvent = false;
    foundSensorEvent = false;

    EventHelper::fill(&userEvent);

    SaErrorT error = saHpiEventAdd(sessionId, &userEvent);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_ADD, SA_OK, error);
    } else {
        status.add(TRACE,
                EventHelper::generateSensorChangeEvent(sessionId, NULL,
                                                       NULL, &matcher));
        if (status.isOkay()) {
            TimeMsec duration = MAX_DURATION;
            while (duration > 0) {
                TimeMsec startTime = getCurrentTime();
                error = saHpiEventGet(sessionId, TIMEOUT, &event, NULL, NULL, NULL);
                if (error == SA_ERR_HPI_TIMEOUT) {
                    // do nothing
                } else if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_GET, SA_OK, error);
                    break;
                } else if (EventHelper::isEqual(&userEvent, &event)) {
                    foundUserEvent = true;
                    if (foundSensorEvent) {
                        break;
                    }
                } else if (matcher.matches(event)) {
                    foundSensorEvent = true;
                    if (foundUserEvent) {
                        break;
                    }
                }

                TimeMsec elapsedTime = getCurrentTime() - startTime;
                duration -= elapsedTime;
            }

            if (status.isOkay()) {

                status.assertPass();

                if (!foundUserEvent) {
                    status.assertFailure(TRACE, "Did not find user event.");
                }
                if (!foundSensorEvent) {
                    status.assertFailure(TRACE, "Did not find Sensor Enable Change event.");
                }
            }
        }
    }

    return status;
}

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

#include "Severities.h"
#include "EventHelper.h"

using namespace ns_saHpiSubscribe;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define SEVERITY_COUNT 6

// one second (nanosec)
#define TIMEOUT 1000000000ll

// 10 seconds (msec)
#define MAX_DURATION 10000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Severities::Severities(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Severities::getName() {
    return "Severities";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Severities::getDescription() {
    return "Subscription allows the HPI User to get events of all severities.\n"
           "Events with each of the valid severities will be added to the HPI\n"
           "system.  Each should be received from the subscription.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Severities::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add events for each severity.  Verify that those events show up in the
 * event queue.
 *****************************************************************************/

HpiTestStatus Severities::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventT userEvent[SEVERITY_COUNT];
    bool found[SEVERITY_COUNT];
    SaHpiSeverityT severity[] = { SAHPI_CRITICAL, 
                                  SAHPI_MAJOR, 
                                  SAHPI_MINOR, 
                                  SAHPI_INFORMATIONAL, 
                                  SAHPI_OK, 
                                  SAHPI_DEBUG };

    // Add the user events

    for (int i = 0; i < SEVERITY_COUNT; i++) {
        found[i] = false;
        EventHelper::fill(&userEvent[i], severity[i]);
        SaErrorT error = saHpiEventAdd(sessionId, &userEvent[i]);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_ADD, SA_OK, error);
            break;
        }
    }

    // Look for the events in the event queue

    if (status.isOkay()) {
        TimeMsec duration = MAX_DURATION;
        while (duration > 0) {
            TimeMsec startTime = getCurrentTime();
            SaErrorT error = saHpiEventGet(sessionId, TIMEOUT,
                                           &event, NULL, NULL, NULL);
            if (error == SA_ERR_HPI_TIMEOUT) {
                // do nothing
            } else if (error != SA_OK) {
                status.assertError(TRACE, EVENT_GET, SA_OK, error);
                break;
            } else {
                int index = findEvent(&event, userEvent);
                if (index != -1) { 
                    found[index] = true;
                    if (foundAll(found)) {
                        break;
                    }
                }
            }

            TimeMsec elapsedTime = getCurrentTime() - startTime;
            duration -= elapsedTime;
        }

        // For each severity, did we find the user event?

        if (status.isOkay()) {
            status.assertPass();
            for (int i = 0; i < SEVERITY_COUNT; i++) {
                if (!found[i]) {
                    status.assertFailure(TRACE, 
                            "Did not find user event with severity %s", 
                            HpiString::severity(severity[i]));
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Did we find all of the events in the event queue?
 *****************************************************************************/

bool Severities::foundAll(bool found[]) {
    for (int i = 0; i < SEVERITY_COUNT; i++) {
        if (!found[i]) {
            return false;
        }
    }
    return true;
}

/*****************************************************************************
 * Find the index of the user event for the given "event".  If there
 * is no match, return -1.
 *****************************************************************************/

int Severities::findEvent(SaHpiEventT *event, SaHpiEventT eventList[]) {
    for (int i = 0; i < SEVERITY_COUNT; i++) {
        if (EventHelper::isEqual(event, &eventList[i])) {
            return i;
        }
    }
    return -1;
}


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

#include "Clear.h"
#include "EventHelper.h"

using namespace ns_saHpiUnsubscribe;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Clear::Clear(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Clear::getName() {
    return "Clear";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Clear::getDescription() {
    return "Any events that are still in the event queue when\n"
           "this function is called will be cleared from it.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Clear::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * 1. Subscribe
 * 2. Add an event and wait
 * 3. Close subscription.
 * 4. Subscribe again.
 * 5. Look for that user event.  It should not be there.
 *****************************************************************************/

HpiTestStatus Clear::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT userEvent;
    bool foundEvent;

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        EventHelper::fill(&userEvent);
        error = saHpiEventAdd(sessionId, &userEvent);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_ADD, SA_OK, error);
        } else {
            pauseForEventProcessing();

            // we can only hope that the user event is now in the event queue.

            error = saHpiUnsubscribe(sessionId);
            if (error != SA_OK) {
                status.assertError(TRACE, UNSUBSCRIBE, SA_OK, error);
            } else {
                error = saHpiSubscribe(sessionId);
                if (error != SA_OK) {
                    status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
                } else {
                    status.add(TRACE, 
                            EventHelper::waitForEvent(sessionId, &userEvent,
                                                      NULL, NULL, NULL,
                                                      &foundEvent));
                    if (status.isOkay()) {
                        if (!foundEvent) {
                            status.assertPass();
                        } else {
                            status.assertFailure(TRACE, 
                                "Found user event after original subscription was closed.");
                        }
                    }

                    error = saHpiUnsubscribe(sessionId);
                    status.checkError(TRACE, UNSUBSCRIBE, error);
                }
            }
        }
    }

    return status;
}

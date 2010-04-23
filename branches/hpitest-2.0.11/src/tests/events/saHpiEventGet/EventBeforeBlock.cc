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

#include "EventBeforeBlock.h"
#include "EventHelper.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EventBeforeBlock::EventBeforeBlock(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EventBeforeBlock::getName() {
    return "EventBeforeBlock";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *EventBeforeBlock::getDescription() {
    return "Add a user event into the event queue and then call\n"
           "saHpiEventGet() with SAHPI_TIMEOUT_BLOCK.  The call\n"
           "should return immediately.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EventBeforeBlock::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an event to the event queue.  Then call saHpiEventGet() with
 * SAHPI_TIMEOUT_BLOCK.  The call should return immediately.
 *****************************************************************************/

HpiTestStatus EventBeforeBlock::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;

    EventHelper::fill(&event);
    SaErrorT error = saHpiEventAdd(sessionId, &event);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_ADD, SA_OK, error);
    } else {
        pauseForEventProcessing();

        TimeMsec startTime = getCurrentTime();
        error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_BLOCK,
                              &event, NULL, NULL, NULL);
        TimeMsec endTime = getCurrentTime();
        if (error == SA_ERR_HPI_TIMEOUT) {
            status.assertFailure(TRACE, 
                        "Timeout occurred with event(s) in the event queue.");
        } else if (error != SA_OK) {
            status.assertError(TRACE, EVENT_GET, SA_OK, error);
        } else {
            TimeMsec duration = endTime - startTime;
            if (duration <= getEpsilonTime()) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                        "saHpiEventGet() took %ld msec to return the event "
                        "even though an event was in the event queue before "
                        "the call to saHpiEventGet().");
            }
        }
    }

    return status;
}

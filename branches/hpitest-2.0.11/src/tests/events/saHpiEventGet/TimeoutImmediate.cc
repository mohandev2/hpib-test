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

#include "TimeoutImmediate.h"
#include "EventHelper.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TimeoutImmediate::TimeoutImmediate(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TimeoutImmediate::getName() {
    return "TimeoutImmediate";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TimeoutImmediate::getDescription() {
    return "With a timeout of SAHPI_TIMEOUT_IMMEDIATE, saHpiEventGet() will\n"
           "return immediately when there are no events in the event queue.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT TimeoutImmediate::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Verify that SAHPI_TIMEOUT_IMMEDIATE does timeout immediately when there 
 * are not events in the event queue.
 *****************************************************************************/

HpiTestStatus TimeoutImmediate::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;

    int retryCnt = 0;
    while (status.isOkay()) {
        status.add(TRACE, EventHelper::clearQueue(sessionId));
        if (status.isOkay()) {
            TimeMsec startTime = getCurrentTime();
            SaErrorT error = saHpiEventGet(sessionId,
                                           SAHPI_TIMEOUT_IMMEDIATE, &event,
                                           NULL, NULL, NULL);
            TimeMsec endTime = getCurrentTime();
            if (error == SA_ERR_HPI_TIMEOUT) {
                TimeMsec duration = endTime - startTime;
                if (duration <= getEpsilonTime()) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, 
                        "saHpiEventGet() did not timeout immediately.\n"
                        "Rather it timed out after %ld msec.", duration);
                } 
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, EVENT_GET, SA_OK, error);
                break;
            } else if (retryCnt >= getMaxEventRetries()) {
                status.assertError("Test status is undetermined.\n"
                                "Too many system events conflicting with timeout test.");
                break;
            } else {
                retryCnt++;
            }
        }
    }

    return status;
}

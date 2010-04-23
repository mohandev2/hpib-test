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

#include "TimeoutNormal.h"
#include "EventHelper.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TimeoutNormal::TimeoutNormal(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TimeoutNormal::getName() {
    return "TimeoutNormal";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TimeoutNormal::getDescription() {
    return "With a positive timeout, saHpiEventGet() will return after that\n"
           "timeout period when there are no events in the event queue.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT TimeoutNormal::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus TimeoutNormal::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    long testTimeout[] = { 1000, 1500, 2000 };

    for (int i = 0; i < 3 && status.isOkay(); i++) {
        int retryCnt = 0;
        long timeout = testTimeout[i];
        while (true) {
            status.add(TRACE, EventHelper::clearQueue(sessionId));
            if (status.isOkay()) {
                TimeMsec startTime = getCurrentTime();
                SaErrorT error = saHpiEventGet(sessionId,
                                               msecToNano(timeout), &event,
                                               NULL, NULL, NULL);
                TimeMsec endTime = getCurrentTime();
                if (error == SA_ERR_HPI_TIMEOUT) {
                    TimeMsec duration = endTime - startTime;
                    if (isMsecTimeEqual(timeout, duration)) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, 
                            "saHpiEventGet() did not timeout after %ld msec.\n"
                            "Rather it timed out after %ld msec.", timeout, duration);
                    }
                    break;
                } else if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_GET, SA_OK,
                                       SA_ERR_HPI_TIMEOUT, error);
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
    }

    return status;
}

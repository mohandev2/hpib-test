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

#include "ResetTimer.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerReset;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ResetTimer::ResetTimer(char *line) : WatchdogTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ResetTimer::getName() {
    return "ResetTimer";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ResetTimer::getDescription() {
    return "Reset the Watchdog Timer.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ResetTimer::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ResetTimer::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ResetTimer::runWatchdogTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiWatchdogRecT *watchdogRec) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT origWatchdog;

    SaErrorT error = saHpiWatchdogTimerGet(sessionId,
                                           rptEntry->ResourceId, 
                                           watchdogRec->WatchdogNum,
                                           &origWatchdog);
    if (error != SA_OK) {
        status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
    } else if (origWatchdog.Running) {
        status.assertNotSupport();
    } else {
        watchdog = origWatchdog;
        watchdog.InitialCount = 60000;
        watchdog.PreTimeoutInterval = 0;
        watchdog.TimerUseExpFlags = 0;
        error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId,
                                      watchdogRec->WatchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerReset(sessionId, rptEntry->ResourceId,
                                            watchdogRec->WatchdogNum);
            if (error == SA_OK) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
            }

            // Restore the original watchdog timer.

            origWatchdog.TimerUseExpFlags = 0;
            error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId,
                                          watchdogRec->WatchdogNum, &origWatchdog);
            status.checkError(TRACE, WATCHDOG_TIMER_SET, error);
        }
    }

    return status;
}

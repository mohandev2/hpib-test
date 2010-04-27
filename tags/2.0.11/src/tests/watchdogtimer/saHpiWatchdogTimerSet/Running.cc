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

#include "Running.h"
#include "Report.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Running::Running(char *line) : WatchdogTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Running::getName() {
    return "Running";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Running::getDescription() {
    return "If the timer was previously stopped when this saHpiWatchdogTimerSet()\n"
           "is called, then it will remain stopped.  If the timer was previously\n"
           "running, then it will continue to run if the Watchdog->Running field\n"
           "passed is TRUE, or will be stopped if the Watchdog->Running field\n"
           "passed is FALSE.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Running::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Running::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Running::runWatchdogTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiWatchdogRecT *watchdogRec) {
    HpiTestStatus status;
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

        // By using the values from the original watchdog settings, 
        // we avoid the possiblity of invalid data.

        SaHpiWatchdogT watchdog = origWatchdog;
        watchdog.Running = SAHPI_FALSE;
        watchdog.InitialCount = 60000; // 60 seconds (we don't want to expire)
        watchdog.PreTimeoutInterval = 0;
        watchdog.TimerUseExpFlags = 0;

        status.add(TRACE, testWatchdog(sessionId, rptEntry, watchdogRec->WatchdogNum, watchdog));

        // Restore the original values, but don't clear the Expiration flags.

        origWatchdog.TimerUseExpFlags = 0;
        error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                      watchdogRec->WatchdogNum, &origWatchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } 
    }

    return status;
}

/*****************************************************************************
 * Test the watchdog.
 *
 * First, check a watchdog that is not running, i.e. stopped.  Then start
 * the timer and test the running watchdog.
 *****************************************************************************/

HpiTestStatus Running::testWatchdog(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiWatchdogNumT watchdogNum,
                                    SaHpiWatchdogT &watchdog) {
    HpiTestStatus status;

    status.add(TRACE, checkStoppedWatchdog(sessionId, rptEntry->ResourceId,
                                           watchdogNum, watchdog));
    if (status.isOkay()) {
        SaErrorT error = saHpiWatchdogTimerReset(sessionId, rptEntry->ResourceId, watchdogNum);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
        } else {
            status.add(TRACE, checkRunningWatchdog(sessionId, rptEntry->ResourceId,
                                                   watchdogNum, watchdog));
        }
    }

    return status;
}

/*****************************************************************************
 * Check a watchdog that is not running.
 *
 * Set the watchdog with Running set to FALSE and TRUE.  In both cases,
 * when we get the watchdog, Running must be FALSE.
 *****************************************************************************/

HpiTestStatus Running::checkStoppedWatchdog(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId,
                                            SaHpiWatchdogNumT watchdogNum,
                                            SaHpiWatchdogT &watchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT curWatchdog;

    watchdog.Running = SAHPI_FALSE;
    SaErrorT error = saHpiWatchdogTimerSet(sessionId, resourceId, 
                                           watchdogNum, &watchdog);
    if (error != SA_OK) {
        status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
    } else {
        error = saHpiWatchdogTimerGet(sessionId, resourceId, 
                                      watchdogNum, &curWatchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
        } else if (curWatchdog.Running) {
            status.assertFailure(TRACE, "Running was FALSE, but became TRUE "
                                        "after setting it to FALSE.");
        } else {
            watchdog.Running = SAHPI_TRUE;
            error = saHpiWatchdogTimerSet(sessionId, resourceId, 
                                           watchdogNum, &watchdog);
            if (error != SA_OK) {
                status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
            } else {
                error = saHpiWatchdogTimerGet(sessionId, resourceId, 
                                              watchdogNum, &curWatchdog);
                if (error != SA_OK) {
                    status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
                } else if (curWatchdog.Running) {
                    status.assertFailure(TRACE, "Running was FALSE, but became TRUE "
                                                "after setting it to TRUE.");
                } else {
                    status.assertPass();
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test a running watchdog.
 *
 * Set the watchdog with Running set to TRUE and then FALSE.  When set to
 * TRUE, the watchdog should continue running.  When set to FALSE, the
 * watchdog must stop.
 *****************************************************************************/

HpiTestStatus Running::checkRunningWatchdog(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId,
                                            SaHpiWatchdogNumT watchdogNum,
                                            SaHpiWatchdogT &watchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT curWatchdog;

    watchdog.Running = SAHPI_TRUE;
    SaErrorT error = saHpiWatchdogTimerSet(sessionId, resourceId, 
                                           watchdogNum, &watchdog);
    if (error != SA_OK) {
        status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
    } else {
        error = saHpiWatchdogTimerGet(sessionId, resourceId, 
                                      watchdogNum, &curWatchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
        } else if (!curWatchdog.Running) {
            status.assertFailure(TRACE, "Running was TRUE, but became FALSE "
                                        "after setting it to TRUE.");
        } else {
            watchdog.Running = SAHPI_FALSE;
            error = saHpiWatchdogTimerSet(sessionId, resourceId, 
                                           watchdogNum, &watchdog);
            if (error != SA_OK) {
                status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
            } else {
                error = saHpiWatchdogTimerGet(sessionId, resourceId, 
                                              watchdogNum, &curWatchdog);
                if (error != SA_OK) {
                    status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
                } else if (curWatchdog.Running) {
                    status.assertFailure(TRACE, "Running was TRUE, but stayed TRUE "
                                                "after setting it to FALSE.");
                } else {
                    status.assertPass();
                }
            }
        }
    }

    return status;
}

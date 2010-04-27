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

#include "Restart.h"
#include "Report.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerSet;

/*****************************************************************************
 * Constants 
 *****************************************************************************/

#define ORIG_INITIAL_COUNT 60000 
#define NEW_INITIAL_COUNT 1200000 

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Restart::Restart(char *line) : WatchdogTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Restart::getName() {
    return "Restart";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Restart::getDescription() {
    return "If a timer is running and it is changed, but continues to run, "
           "it will restart its count-down from the newly configured InitialCount.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Restart::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Restart::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Restart::runWatchdogTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiWatchdogRecT *watchdogRec) {
    HpiTestStatus status;
    SaHpiWatchdogT origWatchdog;
    SaHpiWatchdogT curWatchdog;

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
        watchdog.InitialCount = ORIG_INITIAL_COUNT;
        watchdog.PreTimeoutInterval = 0;
        watchdog.TimerUseExpFlags = 0;

        error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                      watchdogRec->WatchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerReset(sessionId, rptEntry->ResourceId, watchdogRec->WatchdogNum);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
            } else {
                watchdog.Running = SAHPI_TRUE;
                watchdog.InitialCount = NEW_INITIAL_COUNT;
                error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                              watchdogRec->WatchdogNum, &watchdog);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
                } else {
                    // Verify that the PresentCount has reset.  Because we increased
                    // the initial count, the PresentCount has be greater than the
                    // original initial count.

                    error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                                  watchdogRec->WatchdogNum, &curWatchdog);
                    if (error != SA_OK) {
                        status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
                    } else if (curWatchdog.PresentCount <= ORIG_INITIAL_COUNT) {
                        status.assertFailure(TRACE, "The timer was not restarted.");
                    } else {
                        status.assertPass();
                    }
                }
            }
        }
    
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


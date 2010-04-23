/*
 * (C) Copyright University of New Hampshire, 2006
 * (C) Copyright Emerson Network Power, 2009
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
 *     Klaus Mittermaier <klaus.mittermaier@emerson.com>
 *     Lars Wetzel <lars.wetzel@emerson.com>
 */

#include "SetTimer.h"
#include "Report.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetTimer::SetTimer(char *line) : WatchdogTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetTimer::getName() {
    return "SetTimer";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetTimer::getDescription() {
    return "Set the watchdog timer configuration.  The Log, TimerUse, \n"
            "TimerAction, PretimerInterrupt, PreTimeoutInterval, and \n"
            "InitialCount fields are tested to verify that they can be\n"
            "successfully set.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetTimer::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetTimer::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetTimer::runWatchdogTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiWatchdogRecT *watchdogRec) {
    HpiTestStatus status;
    Report report;
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

      	// If the original timer has an invalid timer entry, OEM will be written back instead
      if ((origWatchdog.TimerUse == SAHPI_WTU_NONE) ||
	  (origWatchdog.TimerUse == SAHPI_WTU_UNSPECIFIED))
	origWatchdog.TimerUse = SAHPI_WTU_OEM;


        // We will pass in the original watchdog in order to use its value
        // for the fields that we are not testing.  By using the original
        // values, we avoid the possiblity rejecting a value.

        status.add(TRACE, testLog(sessionId, rptEntry, watchdogRec->WatchdogNum, origWatchdog));
        status.add(TRACE, testTimerUse(sessionId, rptEntry, watchdogRec->WatchdogNum, origWatchdog));
        status.add(TRACE, testTimerAction(sessionId, rptEntry, watchdogRec->WatchdogNum, origWatchdog));
        status.add(TRACE, testPretimerInterrupt(sessionId, rptEntry, watchdogRec->WatchdogNum, origWatchdog));
        status.add(TRACE, testPreTimeoutInterval(sessionId, rptEntry, watchdogRec->WatchdogNum, origWatchdog));
        status.add(TRACE, testInitialCount(sessionId, rptEntry, watchdogRec->WatchdogNum, origWatchdog));

        // Restore the original values, but don't clear the Expiration flags.
        // Also, if the timer was originally running, do a reset to make sure
        // it continues running.

        origWatchdog.TimerUseExpFlags = 0;
        error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                      watchdogRec->WatchdogNum, &origWatchdog);

        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else if (origWatchdog.Running) {
            saHpiWatchdogTimerReset(sessionId, rptEntry->ResourceId, 
                                    watchdogRec->WatchdogNum);
        }
    }

    return status;
}

/*****************************************************************************
 * Test the Log Field.  Try enabling and disabling logging and verify that
 * the set was succesful.
 *****************************************************************************/

HpiTestStatus SetTimer::testLog(SaHpiSessionIdT sessionId,
                                SaHpiRptEntryT *rptEntry,
                                SaHpiWatchdogNumT watchdogNum,
                                SaHpiWatchdogT &origWatchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;
    SaHpiBoolT log[] = { SAHPI_TRUE, SAHPI_FALSE };

    watchdog = origWatchdog;
    watchdog.TimerUseExpFlags = 0;
    for (int i = 0; i < 2 && status.isOkay(); i++) {
        watchdog.Log = log[i];
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                               watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {

            error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                          watchdogNum, &curWatchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
                break;
            } else if (curWatchdog.Log == log[i]) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                    "Failed to set Log to %s.", HpiString::boolean(log[i]));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test the TimerUse Field.  Test all of the possible values.
 * It should be noted that all implementations must accept NONE,
 * but can reject other values.
 *****************************************************************************/

HpiTestStatus SetTimer::testTimerUse(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiWatchdogNumT watchdogNum,
                                     SaHpiWatchdogT &origWatchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;
    SaHpiWatchdogTimerUseT timerUse[] = { SAHPI_WTU_NONE,
                                          SAHPI_WTU_BIOS_FRB2,
                                          SAHPI_WTU_BIOS_POST,
                                          SAHPI_WTU_OS_LOAD,
                                          SAHPI_WTU_SMS_OS,
                                          SAHPI_WTU_OEM,
                                          SAHPI_WTU_UNSPECIFIED };

    status.assertPass();
    watchdog = origWatchdog;
    watchdog.TimerUseExpFlags = 0;
    for (int i = 0; i < 7 && status.isOkay(); i++) {
        watchdog.TimerUse = timerUse[i];
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                               watchdogNum, &watchdog);
        if (error == SA_ERR_HPI_INVALID_DATA) {
        
            // for IPMI ipmlementation SAHPI_WTU_NONE is a reserved value,
            // therefore also SAHPI_WTU_NONE can return an error
            // also SAHPI_WTU_UNSPECIFIED is not an allowed value for timerUse
            // OLD COMMENT: this is okay only if timerUse is not NONE.
            if ((timerUse[i] != SAHPI_WTU_NONE) && (timerUse[i] != SAHPI_WTU_UNSPECIFIED)) {
                status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error, 
                            "TimerUser is %s.", HpiString::watchdogTimerUse(timerUse[i]));
            }
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                          watchdogNum, &curWatchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (curWatchdog.TimerUse != timerUse[i]) {
                status.assertFailure(TRACE, "Failed to set TimerUse to %s.",
                                     HpiString::watchdogTimerUse(timerUse[i]));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test the TimerAction Field.  Test all of the possible values.
 * It should be noted that all implementations must accept NO_ACTION,
 * but can reject other values.
 *****************************************************************************/

HpiTestStatus SetTimer::testTimerAction(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiWatchdogNumT watchdogNum,
                                        SaHpiWatchdogT &origWatchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;
    SaHpiWatchdogActionT timerAction[] = { SAHPI_WA_NO_ACTION,
                                           SAHPI_WA_RESET,
                                           SAHPI_WA_POWER_DOWN,
                                           SAHPI_WA_POWER_CYCLE };

    WatchdogHelper::fill(&watchdog);
    watchdog = origWatchdog;
    watchdog.TimerUseExpFlags = 0;
    for (int i = 0; i < 4 && status.isOkay(); i++) {
        watchdog.TimerAction = timerAction[i];
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                               watchdogNum, &watchdog);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            if (timerAction[i] == SAHPI_WA_NO_ACTION) {
                status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error, 
                                     "TimerAction is SAHPI_WA_NO_ACTION.");
            }
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                          watchdogNum, &curWatchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (curWatchdog.TimerAction == timerAction[i]) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                    "Failed to set TimerAction to %s.",
                    HpiString::watchdogAction(timerAction[i]));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test the PretimerInterrupt Field.  Test all of the possible values.
 * It should be noted that all implementations must accept NONE,
 * but can reject other values.
 *****************************************************************************/

HpiTestStatus SetTimer::testPretimerInterrupt(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiWatchdogNumT watchdogNum,
                                              SaHpiWatchdogT &origWatchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;
    SaHpiWatchdogPretimerInterruptT pretimerInterrupt[] = { SAHPI_WPI_NONE,
                                                            SAHPI_WPI_SMI,
                                                            SAHPI_WPI_NMI,
                                                            SAHPI_WPI_MESSAGE_INTERRUPT,
                                                            SAHPI_WPI_OEM };

    watchdog = origWatchdog;
    watchdog.TimerUseExpFlags = 0;
    for (int i = 0; i < 5 && status.isOkay(); i++) {
        watchdog.PretimerInterrupt = pretimerInterrupt[i];
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                               watchdogNum, &watchdog);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            if (pretimerInterrupt[i] == SAHPI_WPI_NONE) {
                status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error, 
                                     "PretimerInterrupt is SAHPI_WPI_NONE.");
            }
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                          watchdogNum, &curWatchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (curWatchdog.PretimerInterrupt == pretimerInterrupt[i]) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE,
				     "Failed to set PretimerInterrupt to %s.",
                    HpiString::watchdogPretimerInterrupt(pretimerInterrupt[i]));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test the PreTimeoutInterval Field.  Change the value and verify that
 * the set was successful.
 *****************************************************************************/

HpiTestStatus SetTimer::testPreTimeoutInterval(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry,
                                               SaHpiWatchdogNumT watchdogNum,
                                               SaHpiWatchdogT &origWatchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;

    watchdog = origWatchdog;
    watchdog.TimerUseExpFlags = 0;
    
    // original values don't meet the IPMI requirements --> use different values
    // watchdog.PreTimeoutInterval = 998;
    // watchdog.InitialCount = 999;
    watchdog.PreTimeoutInterval = 1000;
    watchdog.InitialCount = 2000;
    
    SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                           watchdogNum, &watchdog);
    if (error != SA_OK) {
        status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
    } else {
        error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                         watchdogNum, &curWatchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
        } else if (curWatchdog.PreTimeoutInterval != watchdog.PreTimeoutInterval) {
            status.assertFailure(TRACE, 
                "Failed to set PreTimeoutInterval to %d.", watchdog.PreTimeoutInterval);
        } else if (curWatchdog.InitialCount != watchdog.InitialCount) {
            status.assertFailure(TRACE, 
                "Failed to set InitialCount to %d.", watchdog.InitialCount);
        } else {
            status.assertPass();
        }
    }

    return status;
}

/*****************************************************************************
 * Test the InitialCount Field.  Change the value and verify that
 * the set was successful.
 *****************************************************************************/

HpiTestStatus SetTimer::testInitialCount(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiWatchdogNumT watchdogNum,
                                         SaHpiWatchdogT &origWatchdog) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;

    watchdog = origWatchdog;
    watchdog.TimerUseExpFlags = 0;
    watchdog.PreTimeoutInterval = 0;
    // original values don't meet the IPMI requirements --> use different values    
    // watchdog.InitialCount = 999;
    watchdog.InitialCount = 1000;
    
    SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                           watchdogNum, &watchdog);
    if (error != SA_OK) {
        status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
    } else {
        error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                      watchdogNum, &curWatchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
        } else if (curWatchdog.InitialCount == watchdog.InitialCount) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, 
                "Failed to set InitialCount to %d.", watchdog.InitialCount);
        }
    }

    return status;
}


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
 *
 */

#include "ExpirationFlags.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogFunctional;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ExpirationFlags::ExpirationFlags(char *line)
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ExpirationFlags::getName() {
    return "ExpirationFlags";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ExpirationFlags::getDescription() {
    return "Verify that the correct Expiration flag is set for the given\n"
           "TimerUse upon expiration of the timer.  Also verify that the\n"
           "individual Expiration bits can be cleared.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ExpirationFlags::getPrecondition() {
    return "Requires one or more watchdogs that are not running and\n"
           "for which a TimerUse of BIOS_FRB2, BIOS_POST, OS_LOAD,\n"
           "SMS_OS, or OEM is accepted by the implementation.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::runSessionTest(SaHpiSessionIdT sessionId) {

    HpiTestStatus status;

    watchdogCount = 0;
    status.add(TRACE, WatchdogHelper::findAllWatchdogs(sessionId, this));
    if (status.isOkay()) {
        if (watchdogCount == 0) {
            status.assertNotSupport();
        } else {
            status.assertPass();
            status.add(TRACE, testWatchdogs(sessionId));
        }
    }

    return status;
}

/*****************************************************************************
 * When findAllWatchdogs finds a watchdog, this method is invoked.
 * We will only test watchdogs that are not running.
 *****************************************************************************/

void ExpirationFlags::addWatchdog(SaHpiSessionIdT sessionId,
				  SaHpiResourceIdT resourceId,
                                  SaHpiWatchdogNumT watchdogNum,
                                  SaHpiWatchdogT &watchdog) {

    if (watchdogCount < MAX_WATCHDOGS && !watchdog.Running) {
        watchdogData[watchdogCount].resourceId = resourceId;
        watchdogData[watchdogCount].watchdogNum = watchdogNum;
        watchdogData[watchdogCount].origWatchdog = watchdog;
        watchdogData[watchdogCount].timerUseExpFlags = 0;
        watchdogCount++;
    }
}

/*****************************************************************************
 * Test each watchdog.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::testWatchdogs(SaHpiSessionIdT sessionId) {

    HpiTestStatus status;
    SaHpiWatchdogTimerUseT timerUse[] = { SAHPI_WTU_BIOS_FRB2,
                                          SAHPI_WTU_BIOS_POST,
                                          SAHPI_WTU_OS_LOAD,
                                          SAHPI_WTU_SMS_OS,
                                          SAHPI_WTU_OEM };

    // First, clear the Expiration flags for all of the
    // watchdogs.  We are running a test to set the bits
    // and so we need to clear those bits.

    status.add(TRACE, clearExpirationFlags(sessionId));

    if (status.isOkay()) {
        for (int i = 0; i < 5 && status.isOkay(); i++) {

            // for each watchdog, start its timer.  We don't want
            // to wait around, so just do an InitialCount of 100.

            for (int j = 0; j < watchdogCount && status.isOkay(); j++) {
                status = startWatchdogTimer(sessionId, &watchdogData[j], timerUse[i], 100);
            }

            // Before continuing, we need to wait for the current timers
            // to expire.  Sleep for 2 seconds which is more than enough 
            // time.
            secSleep(2);
        }

        // If all has gone well so far, all of the watchdog timers
        // have expired.  We can now examine the Expiration flags.

        if (status.isOkay()) {

            // There is one special case we must consider.  Since it
            // is legal for an implementation to reject all of the
            // TimerUse options, we might have failed to set any of
            // the Expiration flags.  Check to see if at least one
            // watchdog is supposed to have one or more Expiration 
            // bits set.

            bool canTest = false;
            for (int j = 0; j < watchdogCount && status.isOkay(); j++) {
                if (watchdogData[j].timerUseExpFlags != 0) {
                    canTest = true;
                    break;
                }
            }

            // If we encountered that special case, we set NOT SUPPORT;
            // otherwise, we can verify the Expiration flags.

            if (!canTest) {
                status.assertNotSupport();
            } else {
                status.add(TRACE, verifyExpirationFlags(sessionId));
            }
        }
    }

    status.add(TRACE, restoreWatchdogs(sessionId));

    return status;
}

/*****************************************************************************
 * Clear the Expiration flags for each of the watchdog timers.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::clearExpirationFlags(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;

    WatchdogHelper::fill(&watchdog);
    watchdog.TimerUseExpFlags = 0x3E; // clear all flags

    for (int i = 0; i < watchdogCount && status.isOkay(); i++) {
        pushLocation("Resource", watchdogData[i].resourceId);
        pushLocation("Watchdog", watchdogData[i].watchdogNum);

        // Use the original TimerUse; it should be accepted.
        watchdog.TimerUse = watchdogData[i].origWatchdog.TimerUse;
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, watchdogData[i].resourceId,
                                               watchdogData[i].watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerGet(sessionId, watchdogData[i].resourceId, 
                                          watchdogData[i].watchdogNum, &curWatchdog);

            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (curWatchdog.TimerUseExpFlags != 0) {
                status.assertFailure(TRACE, "Timer Use Expiration Flags not reset!");
            }
        }

        popLocation();
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Start the Watchdog Timer.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::startWatchdogTimer(SaHpiSessionIdT sessionId,
                                                  WatchdogData *data,
                                                  SaHpiWatchdogTimerUseT timerUse, 
                                                  SaHpiUint32T initialCount) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;

    pushLocation("Resource", data->resourceId);
    pushLocation("Watchdog", data->watchdogNum);

    WatchdogHelper::fill(&watchdog);
    watchdog.TimerUse = timerUse;
    watchdog.InitialCount = initialCount;
    SaErrorT error = saHpiWatchdogTimerSet(sessionId, data->resourceId, 
                                           data->watchdogNum, &watchdog);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        // It would appear that the given TimerUse is not supported
        // on this platform.  This is not an error though.
    } else if (error != SA_OK) {
        status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
    } else {
        error = saHpiWatchdogTimerReset(sessionId, data->resourceId, 
                                        data->watchdogNum);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
        } else {

            // If we were able to start the timer, then keep track of which
            // expiration bit we expect to be set by the implementation.

            if (timerUse == SAHPI_WTU_BIOS_FRB2) {
                data->timerUseExpFlags |= SAHPI_WATCHDOG_EXP_BIOS_FRB2;
            } else if (timerUse == SAHPI_WTU_BIOS_POST) {
                data->timerUseExpFlags |= SAHPI_WATCHDOG_EXP_BIOS_POST;
            } else if (timerUse == SAHPI_WTU_OS_LOAD) {
                data->timerUseExpFlags |= SAHPI_WATCHDOG_EXP_OS_LOAD;
            } else if (timerUse == SAHPI_WTU_SMS_OS) {
                data->timerUseExpFlags |= SAHPI_WATCHDOG_EXP_SMS_OS;
            } else if (timerUse == SAHPI_WTU_OEM) {
                data->timerUseExpFlags |= SAHPI_WATCHDOG_EXP_OEM;
            }
        }
    }

    popLocation();
    popLocation();

    return status;
}

/*****************************************************************************
 * Verify that the expected Expiration flags were set.
 * 
 * Simply compare the expected bit flags to the bit flags that we get.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::verifyExpirationFlags(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiWatchdogT curWatchdog;

    for (int i = 0; i < watchdogCount && status.isOkay(); i++) {

        pushLocation("Resource", watchdogData[i].resourceId);
        pushLocation("Watchdog", watchdogData[i].watchdogNum);

        SaErrorT error = saHpiWatchdogTimerGet(sessionId, watchdogData[i].resourceId,
                                               watchdogData[i].watchdogNum, &curWatchdog);

        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
        } else if (curWatchdog.TimerUseExpFlags != watchdogData[i].timerUseExpFlags) {
            status.assertFailure(TRACE, "TimerUseExpFlags is 0x%x; expected 0x%x!",
                                 curWatchdog.TimerUseExpFlags, watchdogData[i].timerUseExpFlags);
        } else {
            status.add(TRACE, clearExpirationFlagsOneByOne(sessionId, &watchdogData[i]));
        }

        popLocation();
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Clear the Expiration flags one by one.
 * 
 * This is actually a separate test, but I'm piggybacking because I would
 * have to duplicate all of the above.  The above code simply sets all of
 * the Expiration flags that it possibly can.  That is exactly what needs to
 * be done in order to clear the flags one by one.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::clearExpirationFlagsOneByOne(SaHpiSessionIdT sessionId,
                                                            WatchdogData *data) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    SaHpiWatchdogT curWatchdog;
    SaHpiWatchdogExpFlagsT flag[] = { SAHPI_WATCHDOG_EXP_BIOS_FRB2,
                                      SAHPI_WATCHDOG_EXP_BIOS_POST,
                                      SAHPI_WATCHDOG_EXP_OS_LOAD,
                                      SAHPI_WATCHDOG_EXP_SMS_OS,
                                      SAHPI_WATCHDOG_EXP_OEM };

    // for each Expiration flag that was set as expected, clear it.

    watchdog = data->origWatchdog;
    for (int i = 0; i < 5 && status.isOkay(); i++) {
        if (data->timerUseExpFlags & flag[i]) {

            watchdog.TimerUseExpFlags = flag[i];

            SaErrorT error = saHpiWatchdogTimerSet(sessionId, data->resourceId, 
                                                   data->watchdogNum, &watchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
            } else {
                error = saHpiWatchdogTimerGet(sessionId, data->resourceId, 
                                              data->watchdogNum, &curWatchdog);

                if (error != SA_OK) {
                    status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
                } else if (curWatchdog.TimerUseExpFlags & flag[i]) {
                    status.assertFailure(TRACE, "%s was not reset in the TimerUseExpFlags!", 
                                         HpiString::watchdogTimerUseExpFlag(flag[i]));
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Restore the watchdogs to the original settings, except for the Expiration
 * flags which will be cleared.
 *****************************************************************************/

HpiTestStatus ExpirationFlags::restoreWatchdogs(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    for (int i = 0; i < watchdogCount; i++) {
        SaHpiWatchdogT watchdog = watchdogData[i].origWatchdog;
        bool wasRunning = watchdog.Running;
        watchdog.Running = SAHPI_FALSE;
        watchdog.TimerUseExpFlags = 0x3E;
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, watchdogData[i].resourceId, 
                                               watchdogData[i].watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else if (wasRunning) {
            error = saHpiWatchdogTimerReset(sessionId, watchdogData[i].resourceId, 
                                            watchdogData[i].watchdogNum);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
            }
        }
    }

    return status;
}

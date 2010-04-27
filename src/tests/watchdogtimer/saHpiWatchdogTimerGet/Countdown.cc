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
 *     Lars Wetzel <Lars.Wetzel@emerson.com>
 */

#include "Countdown.h"
#include "Report.h"

using namespace ns_saHpiWatchdogTimerGet;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define INITIAL_COUNT_SEC 10
#define INITIAL_COUNT (INITIAL_COUNT_SEC * 1000)

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Countdown::Countdown(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Countdown::getName() {
    return "Countdown";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Countdown::getDescription() {
    return "For a running watchdog timer, verify that that Running field\n"
           "is SAHPI_TRUE and that the PresentCount is decreasing correctly.\n"
           "After the timer expires, verify that Running is SAHPI_FALSE.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Countdown::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Run the test.
 *
 * Gather up all of the watchdog timers and then test them.
 *****************************************************************************/

HpiTestStatus Countdown::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    watchdogCount = 0;

    status.add(TRACE, WatchdogHelper::findAllWatchdogs(sessionId, this));
    if (status.isOkay()) {
        if (watchdogCount == 0) {
            status.assertNotSupport();
        } else {
            status.add(TRACE, testWatchdogs(sessionId));
        }
    }

    return status;
}

/*****************************************************************************
* When findAllWatchdogs finds a watchdog, this method is invoked.
* We will only test watchdogs that are not running.
*****************************************************************************/

void Countdown::addWatchdog(SaHpiSessionIdT sessionId,
			    SaHpiResourceIdT resourceId,
                            SaHpiWatchdogNumT watchdogNum,
                            SaHpiWatchdogT &watchdog) {

    if (watchdogCount < MAX_WATCHDOGS && !watchdog.Running) {
        watchdogData[watchdogCount].resourceId = resourceId;
        watchdogData[watchdogCount].watchdogNum = watchdogNum;
        watchdogData[watchdogCount].origWatchdog = watchdog;
        watchdogCount++;
    }
}

/*****************************************************************************
 * Test the watchdogs.
 *
 * Start all of the timers and then check them a couple of times each
 * to verify that Running is TRUE and that the PresentCount is decreasing.
 * Also, wait for the timers to expire and verify that Running is FALSE.
 *****************************************************************************/

HpiTestStatus Countdown::testWatchdogs(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;

    status.add(TRACE, startTimers(sessionId));

    for (int interval = 0; interval < 2 && !status.hasFault(); interval++) {

        // Let's wait a couple of seconds; want to give the
        // PresentCount a chance to really decrease.

        secSleep(2);

        // Now check each watchdog timer to verify that PresentCount
        // is less than its previous value and that Running is TRUE.

        for (int i = 0; i < watchdogCount && !status.hasFault(); i++) {

            SaHpiResourceIdT resourceId = watchdogData[i].resourceId;
            SaHpiWatchdogNumT watchdogNum = watchdogData[i].watchdogNum;

            pushLocation("Resource", resourceId);
            pushLocation("Watchdog", watchdogNum);

            SaErrorT error = saHpiWatchdogTimerGet(sessionId, resourceId, 
                                                   watchdogNum, &watchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (!watchdog.Running) {
                status.assertFailure(TRACE, "Watchdog Timer should be running, "
                                            "but Running field is SAHPI_FALSE.");
            } else if (watchdog.PresentCount >= watchdogData[i].currentCount) {
                status.assertFailure(TRACE, "PresentCount is not decreasing.");
            }
            watchdogData[i].currentCount = watchdog.PresentCount;

            popLocation();
            popLocation();
        }
    }

    if (status.isOkay()) {

        // Sleep for a while to wait for the timers to expire.
        secSleep(INITIAL_COUNT_SEC);

        // Verify that Running is now FALSE.

        for (int i = 0; i < watchdogCount && !status.hasFault(); i++) {
            SaHpiResourceIdT resourceId = watchdogData[i].resourceId;
            SaHpiWatchdogNumT watchdogNum = watchdogData[i].watchdogNum;

            pushLocation("Resource", resourceId);
            pushLocation("Watchdog", watchdogNum);

            SaErrorT error = saHpiWatchdogTimerGet(sessionId, resourceId, 
                                                   watchdogNum, &watchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (!watchdog.Running) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
				     "Watchdog Timer of resource %d should be stopped, but Running field is SAHPI_TRUE.", 
				     resourceId);
            }

            popLocation();
            popLocation();
        }
    }

    status.add(TRACE, restoreWatchdogs(sessionId));

    return status;
}

/*****************************************************************************
 * Start all of the watchdog timers.
 *****************************************************************************/

HpiTestStatus Countdown::startTimers(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;

    watchdog.Log = SAHPI_TRUE;
    watchdog.Running = SAHPI_FALSE;
    watchdog.TimerAction = SAHPI_WA_NO_ACTION;
    watchdog.PretimerInterrupt = SAHPI_WPI_NONE;
    watchdog.InitialCount = INITIAL_COUNT;
    watchdog.PreTimeoutInterval = 0;
    //code change: initialising TimerUseExpFlags 
    watchdog.TimerUseExpFlags =  (SaHpiWatchdogExpFlagsT) 0;

    for (int i = 0; i < watchdogCount && !status.hasFault(); i++) {
        SaHpiResourceIdT resourceId = watchdogData[i].resourceId;
        SaHpiWatchdogNumT watchdogNum = watchdogData[i].watchdogNum;

	pushLocation("Resource", resourceId);
        pushLocation("Watchdog", watchdogNum);

        watchdogData[i].currentCount = watchdog.InitialCount;
	watchdog.TimerUse =  SAHPI_WTU_OEM;
	//        watchdog.TimerUse = watchdogData[i].origWatchdog.TimerUse;

        SaErrorT error = saHpiWatchdogTimerSet(sessionId, resourceId, 
                                               watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {
            error = saHpiWatchdogTimerReset(sessionId, resourceId, watchdogNum);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
            }
        }

        popLocation();
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Restore the watchdogs to the original settings, but clear the expiration
 * flag for the TimerUse that we used.
 *****************************************************************************/

HpiTestStatus Countdown::restoreWatchdogs(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    for (int i = 0; i < watchdogCount; i++) {
        SaHpiWatchdogT watchdog = watchdogData[i].origWatchdog;
        watchdog.Running = SAHPI_FALSE;
        watchdog.TimerUseExpFlags = WatchdogHelper::getExpirationFlag(watchdog.TimerUse);
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, watchdogData[i].resourceId, 
                                               watchdogData[i].watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } 
    }

    return status;
}

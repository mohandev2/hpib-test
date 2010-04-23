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

#include "PretimerEvent.h"
#include "ListIterator.h"
#include "EventMatcher.h"
#include "EventHelper.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogFunctional;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define POLL_COUNT 10
#define INITIAL_COUNT_SEC 4
#define INITIAL_COUNT (INITIAL_COUNT_SEC * 1000)
#define PRE_TIMEOUT_INTERVAL  (INITIAL_COUNT / 2)
#define TIMER_USE_IN_TC SAHPI_WTU_OEM

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PretimerEvent::PretimerEvent(char *line)
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *PretimerEvent::getName() {
    return "PretimerEvent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *PretimerEvent::getDescription() {
    return "Generate Pretimer Watchdog Events as well as regular Watchdog\n"
           "Events. The Pretimer event will occur before the expiration event.\n"
           "The TimerAction is SAHPI_WA_NO_ACTION and the PretimerInterrupt\n"
           "is SAHPI_WPI_NONE.\n";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *PretimerEvent::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Run the test.
 *
 * Subscribe for events to get the watchdog events.  Then get all of the
 * watchdog timers and test them.
 *****************************************************************************/

HpiTestStatus PretimerEvent::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        watchdogCount = 0;
        foundEventCount = 0;

        status.add(TRACE, WatchdogHelper::findAllWatchdogs(sessionId, this));
        if (status.isOkay()) {
            if (watchdogCount == 0) {
                status.assertNotSupport();
            } else {
                status.assertPass(); // assume all will go well
                status.add(TRACE, testWatchdogs(sessionId));
            }
        }

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}


/*****************************************************************************
 * * When findAllWatchdogs finds a watchdog, this method is invoked.
 * * We will only test watchdogs that are not running.
 * *****************************************************************************/

void PretimerEvent::addWatchdog(SaHpiSessionIdT sessionId,
				SaHpiResourceIdT resourceId,
                                SaHpiWatchdogNumT watchdogNum,
                                SaHpiWatchdogT &origWatchdog) {
  SaHpiWatchdogT watchdog;
  
  // Test values - not each watchdog must support this settings
  watchdog.Log = SAHPI_TRUE;
  watchdog.Running = SAHPI_FALSE;
  watchdog.TimerAction = SAHPI_WA_NO_ACTION;
  watchdog.PretimerInterrupt = SAHPI_WPI_MESSAGE_INTERRUPT;
  watchdog.PreTimeoutInterval = PRE_TIMEOUT_INTERVAL;
  watchdog.InitialCount = INITIAL_COUNT;
  watchdog.TimerUseExpFlags = 0;
  watchdog.TimerUse = TIMER_USE_IN_TC;

  if (watchdogCount < MAX_WATCHDOGS && !origWatchdog.Running) {
   
    SaErrorT error = saHpiWatchdogTimerSet(sessionId, resourceId, 
					   watchdogNum, &watchdog);
      
    if (error == SA_OK) {

      watchdogData[watchdogCount].resourceId = resourceId;
      watchdogData[watchdogCount].watchdogNum = watchdogNum;
      watchdogData[watchdogCount].origWatchdog = origWatchdog;
      watchdogData[watchdogCount].foundPreEvent = false;
      watchdogData[watchdogCount].foundEvent = false;
      watchdogCount++;
    }
  }
}

/*****************************************************************************
 * Test the watchdogs.
 *
 * Start each watchdog timer with a pretimer interrupt.  After they are
 * running, poll for the watchdog events.
 *****************************************************************************/

HpiTestStatus PretimerEvent::testWatchdogs(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;

    watchdog.Log = SAHPI_TRUE;
    watchdog.Running = SAHPI_FALSE;
    watchdog.TimerAction = SAHPI_WA_NO_ACTION;
    watchdog.PretimerInterrupt = SAHPI_WPI_MESSAGE_INTERRUPT;
    watchdog.PreTimeoutInterval = PRE_TIMEOUT_INTERVAL;
    watchdog.InitialCount = INITIAL_COUNT;
    watchdog.TimerUse = TIMER_USE_IN_TC;
    watchdog.TimerUseExpFlags = 0;

    for (int i = 0; i < watchdogCount && !status.hasFault(); i++) {
        SaHpiResourceIdT resourceId = watchdogData[i].resourceId;
        SaHpiWatchdogNumT watchdogNum = watchdogData[i].watchdogNum;

        pushLocation("Resource", resourceId);
        pushLocation("Watchdog", watchdogNum);

        SaErrorT error = saHpiWatchdogTimerSet(sessionId, resourceId, 
                                               watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } else {

	  error = saHpiWatchdogTimerReset(sessionId, resourceId, watchdogNum);
	  if (error != SA_OK) {
	    status.assertError(TRACE, WATCHDOG_TIMER_RESET, SA_OK, error);
	  }
        }
        popLocation();
        popLocation();
    }

    if (status.isOkay()) {

        // check for the watchdog events; we will stop polling
        // after twice the INITIAL_COUNT.

        status.add(TRACE, pollEvents(sessionId, INITIAL_COUNT_SEC * 2)); 
	if (status.isOkay()) {
	  status.add(TRACE, verifyEvents());
	}
     }

    status.add(TRACE, restoreWatchdogs(sessionId));

    return status;
}

/*****************************************************************************
 * Poll for events.
 * 
 * The "timeout" is in seconds.  If it is -1, then the timeout is 
 * immediate.  The Poller will look for Watchdog Events.  When one is found,
 * the below handleEvent() method will be invoked.
 *****************************************************************************/

HpiTestStatus PretimerEvent::pollEvents(SaHpiSessionIdT sessionId, long timeout) {
    HpiTestStatus status;

    EventMatcher eventMatcher;
    eventMatcher.setEventType(SAHPI_ET_WATCHDOG);
    status.add(TRACE, EventHelper::pollEvents(sessionId, timeout,
                                              eventMatcher, this));

    return status;
}

/*****************************************************************************
 * If the Poller encounters a Watchdog Event, this handleEvent() method will
 * be invoked.  When invoked, we will search the Watchdog Data for the
 * watchdog that matches the event.  If we find the watchdog then mark it 
 * so that we know the event was found.
 *****************************************************************************/

HpiTestStatus PretimerEvent::handleEvent(SaHpiSessionIdT, SaHpiEventT *event) {
    HpiTestStatus status;

    // Search our list of watchdogs to find which one this
    // watchdog event corresponds to.

    SaHpiWatchdogEventT *watchdogEvent = &(event->EventDataUnion.WatchdogEvent);
    SaHpiWatchdogNumT watchdogNum = watchdogEvent->WatchdogNum;
    for (int i = 0; i < watchdogCount; i++) {
        if ((watchdogData[i].resourceId == event->Source) && 
            (watchdogData[i].watchdogNum == watchdogNum)) {

            foundEventCount++;
            pushLocation("Resource", event->Source);
            pushLocation("Watchdog", watchdogNum);

            // Do we have the Pretimeout Interrupt Event?  If so, let's
            // same its event timestamp and verify that the PreTimerAction
            // is NONE.

            if (watchdogEvent->WatchdogAction == SAHPI_WAE_TIMER_INT) {
                watchdogData[i].foundPreEvent = SAHPI_TRUE;
                watchdogData[i].preTimestamp = event->Timestamp;
                if (watchdogEvent->WatchdogPreTimerAction != SAHPI_WPI_MESSAGE_INTERRUPT) {
                    status.assertFailure(TRACE, 
                        "Watchdog Event PreTimerAction is %s; expected SAHPI_WPI_MESSAGE_INTERRUPT.", 
                        HpiString::watchdogPretimerInterrupt(watchdogEvent->WatchdogPreTimerAction));
                }
            } else {
                // This must be the normal watchdog event.  Check to see if the
                // Pretimer Interrupt Event occurred.  If the Pretimer Interrupt
                // Event did occur, verify that the interval between the two events
                // is correct.

                watchdogData[i].foundEvent = SAHPI_TRUE;
                if (!watchdogData[i].foundPreEvent) {
                    status.assertFailure(TRACE, 
                        "Watchdog Event occurred BEFORE Pretimer Interrupt Event.");
                } else {
                    SaHpiTimeT interval = (event->Timestamp - watchdogData[i].preTimestamp) / 1000000;
                    if (!isMsecTimeEqual(PRE_TIMEOUT_INTERVAL, interval)) {
                        status.assertFailure(TRACE, 
                            "Watchdog Pretimer Interrupt Event had a PreTimeoutInterval of %d; expected %d.",
                            interval, PRE_TIMEOUT_INTERVAL);
                    }
                }
            }

            // Lastly, for either event, the TimerUse had better correspond to
            // what it was originally set to.

	    if (watchdogEvent->WatchdogUse != TIMER_USE_IN_TC) {
                status.assertFailure(TRACE, 
                    "Watchdog Event Use is %s; expected %s.",
                    HpiString::watchdogTimerUse(watchdogEvent->WatchdogUse),
		    HpiString::watchdogTimerUse(TIMER_USE_IN_TC));
            }

            popLocation();
            popLocation();
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * The isEventProcessingDone() method is used to prematurely stop polling
 * for events.  In this test, we don't need to continue polling the event
 * queue once we have found both events for each watchdog.
 *****************************************************************************/

bool PretimerEvent::isEventProcessingDone() {
    return (foundEventCount == watchdogCount * 2);
}

/*****************************************************************************
 * Verify that we received all of the watchdog events we expected.  If we
 * didn't get an event, report that as a failure.
 *****************************************************************************/

HpiTestStatus PretimerEvent::verifyEvents() {
    HpiTestStatus status;

    for (int i = 0; i < watchdogCount; i++) {

        pushLocation("Resource", watchdogData[i].resourceId);
        pushLocation("Watchdog", watchdogData[i].watchdogNum);

        if (!watchdogData[i].foundPreEvent) {
           status.assertFailure(TRACE, "Did not receive a Watchdog PreTimeout Interrupt Event!");

        } else if (!watchdogData[i].foundEvent) {
            status.assertFailure(TRACE, "Did not receive a Watchdog Event!");
        }

        popLocation();
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Restore the watchdogs to the original settings, but clear the Expriation
 * flag for the TimerUse.
 *****************************************************************************/

HpiTestStatus PretimerEvent::restoreWatchdogs(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    for (int i = 0; i < watchdogCount; i++) {
        SaHpiWatchdogT watchdog = watchdogData[i].origWatchdog;
        watchdog.TimerUseExpFlags = WatchdogHelper::getExpirationFlag(watchdog.TimerUse);
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, watchdogData[i].resourceId, 
                                               watchdogData[i].watchdogNum, &watchdog);
        if (error != SA_OK) {
            status.assertError(TRACE, WATCHDOG_TIMER_SET, SA_OK, error);
        } 
    }

    return status;
}


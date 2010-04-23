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
 * Changes
 *  09/11/01 <larswetzel@users.sourceforge.net>
 *           Eliminate compile warning
 */

#include "Event.h"
#include "ListIterator.h"
#include "EventMatcher.h"
#include "EventHelper.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogFunctional;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define POLL_COUNT 10

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Event::Event(char *line)
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Event::getName() {
    return "Event";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Event::getDescription() {
    return "Generate an event on expiration of the watchdog timer.  Verify\n"
           "that the event is generated and that the event's fields are correct.\n"
           "A TimerAction of NO_ACTION is used.  There is no pretimer interrupt.\n";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Event::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Run the test.
 *
 * Subscribe for events in order to get the watchdog events.  Then
 * gather all of the watchdogs and test them.
 *****************************************************************************/

HpiTestStatus Event::runSessionTest(SaHpiSessionIdT sessionId) {
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
                status.assertPass();  // assume all will go well
                status.add(TRACE, testWatchdogs(sessionId));
            }
        }

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
* When findAllWatchdogs finds a watchdog, this method is invoked.
* We will only test watchdogs that are not running.
*****************************************************************************/

void Event::addWatchdog(SaHpiSessionIdT sessionId,
                        SaHpiResourceIdT resourceId,
                        SaHpiWatchdogNumT watchdogNum,
                        SaHpiWatchdogT &watchdog) {

    if (watchdogCount < MAX_WATCHDOGS && !watchdog.Running) {
        watchdogData[watchdogCount].resourceId = resourceId;
        watchdogData[watchdogCount].watchdogNum = watchdogNum;
        watchdogData[watchdogCount].origWatchdog = watchdog;
        watchdogData[watchdogCount].foundEvent = false;
        watchdogCount++;
    }

    // To overcome a warning of unused sessionId
    saHpiDiscover(sessionId);     
}

/*****************************************************************************
 * Test the watchdogs.
 *
 * Start each watchdog timer and then poll for a while waiting for the
 * watchdog events to show up.
 *****************************************************************************/

HpiTestStatus Event::testWatchdogs(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;

    for (int i = 0; i < watchdogCount && !status.hasFault(); i++) {
        SaHpiResourceIdT resourceId = watchdogData[i].resourceId;
        SaHpiWatchdogNumT watchdogNum = watchdogData[i].watchdogNum;

        pushLocation("Resource", resourceId);
        pushLocation("Watchdog", watchdogNum);

        watchdog.Log = SAHPI_TRUE;
        watchdog.Running = SAHPI_FALSE;
        watchdog.InitialCount = 100;
        watchdog.TimerAction = SAHPI_WA_NO_ACTION;
        watchdog.PretimerInterrupt = SAHPI_WPI_NONE;
        watchdog.PreTimeoutInterval = 0;
        watchdog.TimerUse = watchdogData[i].origWatchdog.TimerUse;
	//code change: TimerUseExpFlags was not initialized before.
        watchdog.TimerUseExpFlags = WatchdogHelper::getExpirationFlag(watchdog.TimerUse);

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

        // Periodically poll for events so that the event queue doesn't overflow.

        if (status.isOkay() && (i % POLL_COUNT == 0)) {
            status.add(TRACE, pollEvents(sessionId, -1));
        }
    }

    if (!status.hasFault()) {

        // Check for any remaining events that may have been generated.
        // We will poll for up to 10 seconds to give the last watchdog 
        // time to generate an event.
    
        status.add(TRACE, pollEvents(sessionId, 10));
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
 * 
 * Note that we minimize the amount of polling we do, so we will only
 * poll when we've added POLL_COUNT watchdogs.
 *****************************************************************************/

HpiTestStatus Event::pollEvents(SaHpiSessionIdT sessionId, long timeout) {
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
 * watchdog matches the event.  If we find the watchdog then mark it so that 
 * we know it was found.
 *****************************************************************************/

HpiTestStatus Event::handleEvent(SaHpiSessionIdT, SaHpiEventT *event) {
    HpiTestStatus status;

    SaHpiWatchdogEventT *watchdogEvent = &(event->EventDataUnion.WatchdogEvent);
    SaHpiWatchdogNumT watchdogNum = watchdogEvent->WatchdogNum;
    for (int i = 0; i < watchdogCount; i++) {
        if ((watchdogData[i].resourceId == event->Source) && 
            (watchdogData[i].watchdogNum == watchdogNum)) {

            foundEventCount++;
            pushLocation("Resource", event->Source);
            pushLocation("Watchdog", watchdogNum);

            watchdogData[i].foundEvent = true;

            if (watchdogEvent->WatchdogAction != SAHPI_WAE_NO_ACTION) {
                status.assertFailure(TRACE, 
                    "Watchdog Event TimerAction is %s; expected SAHPI_WAE_NO_ACTION.", 
                    HpiString::watchdogActionEvent(watchdogEvent->WatchdogAction));
            }

            if (watchdogEvent->WatchdogUse != watchdogData[i].origWatchdog.TimerUse) {
                status.assertFailure(TRACE, 
                    "Watchdog Event TimerUse is %s; expected %s.", 
                    HpiString::watchdogTimerUse(watchdogEvent->WatchdogUse),
                    HpiString::watchdogTimerUse(watchdogData[i].origWatchdog.TimerUse));
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
 * queue once we have found all of the watchdog events.
 *****************************************************************************/

bool Event::isEventProcessingDone() {
    return (foundEventCount == watchdogCount);
}

/*****************************************************************************
 * Verify that we received all of the watchdog events.  If we didn't get 
 * an event, report that as a failure.
 *****************************************************************************/

HpiTestStatus Event::verifyEvents() {
    HpiTestStatus status;

    for (int i = 0; i < watchdogCount; i++) {

        pushLocation("Resource", watchdogData[i].resourceId);
        pushLocation("Watchdog", watchdogData[i].watchdogNum);

        if (!watchdogData[i].foundEvent) {
            status.assertFailure(TRACE, "Did not receive a Watchdog Event!");
        }

        popLocation();
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Restore the watchdogs to the original settings, but clear the
 * Expiration flag for the TimerUse.
 *****************************************************************************/

HpiTestStatus Event::restoreWatchdogs(SaHpiSessionIdT sessionId) {
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


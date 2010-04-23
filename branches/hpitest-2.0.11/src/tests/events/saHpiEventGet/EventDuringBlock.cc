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

#include <config.h>
#include "EventDuringBlock.h"
#include "EventHelper.h"
#include "Time.h"
#include "Timer.h"
#include "TimeoutInfo.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define EVENT_TIMEOUT 3 // seconds

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EventDuringBlock::EventDuringBlock(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EventDuringBlock::getName() {
    return "EventDuringBlock";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *EventDuringBlock::getDescription() {
    return "Call saHpiEventGet() with SAHPI_TIMEOUT_BLOCK and an empty\n"
           "event queue.  During the block, use another thread to add an\n"
           "event to the event queue.  The call to saHpiEventGet() should return.";
}

/*****************************************************************************
 * Return the Precondition of the test case.
 *****************************************************************************/

const char *EventDuringBlock::getPrecondition() {
    return "Requires the support of threads.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EventDuringBlock::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Call saHpiEventGet() with SAHPI_TIMEOUT_BLOCK.  Using a timer, an event
 * is added to the event queue at a later time.  The call to saHpiEventGet()
 * should return with that event.  If a system event appears in the queue,
 * we will retry the test.
 *****************************************************************************/

HpiTestStatus EventDuringBlock::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

#ifndef USE_THREADS
    status.assertNotSupport();
#else
    SaHpiEventT userEvent;
    SaHpiEventT event;
    char buf[100];

    int retryCnt = 0;
    while (status.isOkay()) {

        // Clear out the event queue and cross our fingers that
        // no system events show up in the queue.

        status.add(TRACE, EventHelper::clearQueue(sessionId));
        if (status.isOkay()) {

            // Create the event that we will later add to the event queue.

            sprintf(buf, "Block Test %d", retryCnt);
            EventHelper::fill(&userEvent, buf);
            TimeoutInfo info(sessionId, &userEvent);

            // Start a timer that will be responsible for adding
            // the user event to the event queue.

            int timerId = Timer::add(testTimeoutCallback, EVENT_TIMEOUT, &info);

            // Wait for an event.

            SaErrorT error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_BLOCK,
                                           &event, NULL, NULL, NULL);
         
            Timer::remove(timerId);

            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_GET, SA_OK, error);
            } else if (EventHelper::isEqual(&event, &userEvent)) {
                status.assertPass();
                break;
            } else if (retryCnt >= getMaxEventRetries()) {
                status.assertError(TRACE, 
                        "Status is undetermined.  Never got a user event in the\n"
                        "specified time interval.  Too many system events occured.");
            } else {
                // We must have gotten an unexpected system event.
                // Let's try again.
                retryCnt++;
            }
        }
    }
#endif

    return status;
}

/*****************************************************************************
 * Add an event.
 *****************************************************************************/

void EventDuringBlock::testTimeoutCallback(void *data) {
    TimeoutInfo *info = (TimeoutInfo *) data;
    info->addEvent();
}

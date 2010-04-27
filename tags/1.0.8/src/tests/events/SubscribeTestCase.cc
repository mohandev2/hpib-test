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
#include "SubscribeTestCase.h"
#include "Timer.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

SubscribeTestCase::SubscribeTestCase(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Wraps event subscription around the actual test.  It also sets a timer
 * in case the test doesn't terminate in a reasonable period of time.  In
 * other words, when waiting for event with saHpiEventGet(), it is possible
 * to block for a long period of time.  To prevent this, the subscription
 * will be closed to try to release the block and force the test to exit.
 *****************************************************************************/

HpiTestStatus SubscribeTestCase::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {

#ifdef USE_THREADS
        // Timeout before the system test timeout; we want to try
        // to force the test to terminate successfully without having
        // to abort the entire application.  For example, if saHpiEventGet()
        // is blocked, it is hoped that doing an unsubscribe will force
        // saHpiEventGet() to return which will result in the test
        // terminating.

        int timeout = getSessionTimeout() - EVENT_TIMEOUT_OFFSET;
        int timerId = Timer::add(testTimeoutCallback, timeout, &sessionId);
#endif

        status.add(TRACE, runEventTest(sessionId));

#ifdef USE_THREADS
        Timer::remove(timerId);
#endif

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Callback function to unsubscribe.  If this gets called, it is hoped
 * that closing the subscription will force the block to be released and
 * that the test will finish.
 *****************************************************************************/

void SubscribeTestCase::testTimeoutCallback(void *data) {
    SaHpiSessionIdT sessionId = *((SaHpiSessionIdT *) data);
    saHpiUnsubscribe(sessionId);
}


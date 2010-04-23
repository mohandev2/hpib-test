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
#include "CloseSession.h"
#include "EventHelper.h"
#include "Timer.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define TIMEOUT 2 // second

/*****************************************************************************
 * Constructor
 *****************************************************************************/

CloseSession::CloseSession(char *line) : HpiDomainTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *CloseSession::getName() {
    return "CloseSession";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *CloseSession::getDescription() {
    return "Invoke saHpiEventGet() with a timeout value other than "
           "SAHPI_TIMEOUT_IMMEDIATE, and then close the session from "
           "another thread.";
}

/*****************************************************************************
 * Return the Precondition of the test case.
 *****************************************************************************/

const char *CloseSession::getPrecondition() {
    return "Requires support for threads.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT CloseSession::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_SESSION;
}

/*****************************************************************************
 * Run the test.
 *
 * The test must close the session while waiting for an event.  There is a
 * race condition that must be dealt with: an event could show up in the
 * event queue BEFORE we close the session.  Due to that, we will retry the
 * test multiple times in the hope of being successful.
 *****************************************************************************/

HpiTestStatus CloseSession::runDomainTest(SaHpiDomainIdT domainId) {
    HpiTestStatus status;

#ifndef USE_THREADS
    status.assertNotSupport();
#else
    SaHpiEventT event;
    SaHpiSessionIdT sessionId;

    int retryCnt = 0;
    while (status.isUnknown()) {
        SaErrorT error = saHpiSessionOpen(domainId, &sessionId, NULL);
        if (error != SA_OK) {
            status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
        } else {
            error = saHpiSubscribe(sessionId);
            if (error != SA_OK) {
                status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
            } else {

                // throw out anything that is already in the event queue

                status.add(TRACE, EventHelper::clearQueue(sessionId));
                if (status.isOkay()) {

                    // Wait for the event, but add a timer that will expire 
                    // after two seconds which will cause the session to be closed.

                    int timerId = Timer::add(testTimeoutCallback, TIMEOUT, &sessionId);
                    error = saHpiEventGet(sessionId, secToNano(TIMEOUT * 3),
                                          &event, NULL, NULL, NULL);
                    Timer::remove(timerId);

                    // Getting SA_OK should only occur if an event was returned and thus
                    // the session never got closed.  We will need to retry unless we
                    // have run out of retries.

                    if (error == SA_OK) {
                        if (retryCnt >= getMaxEventRetries()) {
                            status.assertError(TRACE, 
                                "Status is undetermined; saHpiEventGet() continues to return SA_OK.\n"
                                "This may indicate that many system events are occurring and thus\n"
                                "interferring with this test.  It is also possible that saHpiEventGet()\n"
                                "is mistakenly returning SA_OK instead of SA_ERR_HPI_INVALID_SESSION.");
                        }
                        retryCnt++;
                    } else if (error == SA_ERR_HPI_INVALID_SESSION) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, EVENT_GET, SA_ERR_HPI_INVALID_SESSION, error);
                    }
                }
            }

            // don't check error since it may have already been closed
            saHpiSessionClose(sessionId);
        }
    }
#endif

    return status;
}

/*****************************************************************************
 * Close the session.
 *****************************************************************************/

void CloseSession::testTimeoutCallback(void *data) {
    SaHpiSessionIdT *sessionId = (SaHpiSessionIdT *) data;
    saHpiSessionClose(*sessionId);
}


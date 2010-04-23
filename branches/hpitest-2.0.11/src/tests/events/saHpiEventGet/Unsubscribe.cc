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
#include "Unsubscribe.h"
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

Unsubscribe::Unsubscribe(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Unsubscribe::getName() {
    return "Unsubscribe";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Unsubscribe::getDescription() {
    return "Call saHpiEventGet() with a timeout value other than\n"
           "SAHPI_TIMEOUT_IMMEDIATE, and subsequently call saHpiUnsubscribe()\n"
           "from another thread.";
}

/*****************************************************************************
 * Return the Precondition of the test case.
 *****************************************************************************/

const char *Unsubscribe::getPrecondition() {
    return "Requires the support of threads.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Unsubscribe::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *
 * While saHpiEventGet() is waiting for an event, have a separate thread
 * unsubscribe.  This should saHpiEventGet() to return with INVALID_REQUEST.
 * Due to system events, we may need to try this test multiple times.
 *****************************************************************************/

HpiTestStatus Unsubscribe::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

#ifndef USE_THREADS
    status.assertNotSupport();
#else
    SaHpiEventT event;

    int retryCnt = 0;
    while (status.isUnknown()) {
        SaErrorT error = saHpiSubscribe(sessionId);
        if (error != SA_OK) {
            status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
        } else {
            status.add(TRACE, EventHelper::clearQueue(sessionId));
            if (status.isOkay()) {
                int timerId = Timer::add(testTimeoutCallback, TIMEOUT, &sessionId);
                error = saHpiEventGet(sessionId, secToNano(TIMEOUT * 3), 
                                      &event, NULL, NULL, NULL);
                Timer::remove(timerId);

                if (error == SA_OK) {
                    if (retryCnt >= getMaxEventRetries()) {
                        status.assertError(TRACE, 
                            "Status is undetermined; saHpiEventGet() continues to return SA_OK.\n"
                            "This may indicate that many system events are occurring and thus\n"
                            "interferring with this test.  It is also possible that saHpiEventGet()\n"
                            "is mistakenly returning SA_OK instead of SA_ERR_HPI_INVALID_REQUEST.");
                    }
                    retryCnt++;
                } else if (error != SA_ERR_HPI_INVALID_REQUEST) {
                    status.assertFailure(TRACE, EVENT_GET, SA_ERR_HPI_INVALID_REQUEST, error);
                } else {
                    status.assertPass();
                }
            }

            // must make sure we unsubscribed; ignore any errors
            // because we might have unsubscribed in the callback function
            saHpiUnsubscribe(sessionId);
        }
    }
#endif

    return status;
}

/*****************************************************************************
 * Unsubscribe.
 *****************************************************************************/

void Unsubscribe::testTimeoutCallback(void *data) {
    SaHpiSessionIdT *sessionId = (SaHpiSessionIdT *) data;
    saHpiUnsubscribe(*sessionId);
}


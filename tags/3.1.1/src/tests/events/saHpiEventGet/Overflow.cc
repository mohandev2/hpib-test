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

#include "Overflow.h"
#include "EventHelper.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Overflow::Overflow(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Overflow::getName() {
    return "Overflow";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Overflow::getDescription() {
    return "The <i>EventQueueStatus</i> flag indicates whether or not a\n"
           "queue overflow has occurred.  The overflow flag is set if any\n"
           "events were unable to be queued because of space limitations in\n"
           "the interface implementation. The overflow flag is reset\n"
           "whenever saHpiEventGet() is called.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Overflow::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Overflow::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEvtQueueStatusT queueStatus;

    // make sure overflow flag is cleared 
    SaErrorT error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_IMMEDIATE,
                                   &event, NULL, NULL, NULL);

    if (error != SA_OK && error != SA_ERR_HPI_TIMEOUT) {
        status.assertError(TRACE, EVENT_GET, SA_OK, SA_ERR_HPI_TIMEOUT, error);
    } else {

        // Add lots of events in order to overflow the event queue

        EventHelper::fill(&event);
        for (int i = 0; i < getMaxEventQueueSize() + 1; i++) {
            error = saHpiEventAdd(sessionId, &event);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_ADD, SA_OK, error);
                break;
            }
        }

        // Get an event and verify that the overflow flag was set. Then
        // get the next event and verify that the overflow flag was reset.

        if (status.isOkay()) {
            error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_IMMEDIATE,
                                  &event, NULL, NULL, &queueStatus);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_GET, SA_OK, error);
            } else if (queueStatus != SAHPI_EVT_QUEUE_OVERFLOW) {
                status.assertFailure(TRACE, "The overflow flag has not been set.\n"
                            "This may be the result of not adding enough events to\n"
                            "overflow the event queue.  Try increasing the size of\n"
                            "the MaxEventQueueSize property in ~/.hpitest.");
            } else {
                error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_IMMEDIATE,
                                      &event, NULL, NULL, &queueStatus);
                if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_GET, SA_OK, error);
                } else if (queueStatus == SAHPI_EVT_QUEUE_OVERFLOW) {
                    status.assertFailure(TRACE, "The overflow flag was not reset.");
                } else {
                    status.assertPass();
                }
            }
        }

        // By adding lots of events, the domain event log will be filled up.
        // Therefore, let's clear the log.

        error = saHpiEventLogClear(sessionId, SAHPI_UNSPECIFIED_RESOURCE_ID);
        status.checkError(TRACE, EVENT_LOG_CLEAR, error);
    }

    return status;
}

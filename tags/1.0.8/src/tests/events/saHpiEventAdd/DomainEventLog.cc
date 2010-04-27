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

#include "DomainEventLog.h"
#include "EventHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DomainEventLog::DomainEventLog(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DomainEventLog::getName() {
    return "DomainEventLog";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DomainEventLog::getDescription() {
    return "Events will be logged to the Domain Event Log, if room is available.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DomainEventLog::getPrecondition() {
    return "Requires the ability to add an entry to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DomainEventLog::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * The Domain Event Log must be enabled in order to run the test.
 *****************************************************************************/

HpiTestStatus DomainEventLog::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiBoolT enableState;

    SaErrorT error = saHpiEventLogStateGet(sessionId,
                                           SAHPI_UNSPECIFIED_RESOURCE_ID, 
                                           &enableState);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
    } else if (enableState) {
        status.add(TRACE, runAddTest(sessionId));
    } else {
        error = saHpiEventLogStateSet(sessionId,
                                      SAHPI_UNSPECIFIED_RESOURCE_ID,
                                      SAHPI_TRUE);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_STATE_SET, SA_OK, error);
        } else {
            status.add(TRACE, runAddTest(sessionId));

            error = saHpiEventLogStateSet(sessionId,
                                          SAHPI_UNSPECIFIED_RESOURCE_ID,
                                          SAHPI_FALSE);
            status.checkError(TRACE, EVENT_LOG_STATE_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * First, make sure there is room in the Domain Event Log to add a new entry.
 * Then add an HPI User event and wait for it to be processed.  Then look in
 * the Domain Event Log for that event.  It should be found.
 *****************************************************************************/

HpiTestStatus DomainEventLog::runAddTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    bool found;

    status.add(TRACE, EventLogHelper::makeRoom(sessionId,
                                               SAHPI_UNSPECIFIED_RESOURCE_ID, 1));
    if (status.isOkay()) {
        EventHelper::fill(&event);
        SaErrorT error = saHpiEventAdd(sessionId, &event);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_ADD, SA_OK, error);
        } else {
            pauseForEventProcessing();
            status.add(TRACE, EventLogHelper::findEvent(sessionId,
                                                        SAHPI_UNSPECIFIED_RESOURCE_ID,
                                                        &event, &found));
            if (status.isOkay()) {
                if (found) {
                    status.assertPass();
                } else {
                    status.assertPassWarn(TRACE, 
                        "Expected to find the event in the Domain Event Log, "
                        "but that is permitted according to the HPI specification.");
                }
            }
        }
    }

    return status;
}


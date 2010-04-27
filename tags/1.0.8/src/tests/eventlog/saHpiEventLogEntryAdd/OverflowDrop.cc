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

#include "OverflowDrop.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OverflowDrop::OverflowDrop(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OverflowDrop::getName() {
    return "OverflowDrop";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OverflowDrop::getDescription() {
    return "If the Event Log's <i>OverflowAction</i> is SAHPI_EL_OVERFLOW_DROP,\n"
           "then an error will be returned indicating that the event did not\n"
           "get added to the Event Log.  The <i>OverflowFlag</i> is also checked\n"
           "to verify that it was set for an overflow.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OverflowDrop::getPrecondition() {
    return "Requires an Event Log with an OverflowAction of SAHPI_EL_OVERFLOW_DROP.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OverflowDrop::getExpectedReturn() {
    return SA_ERR_HPI_OUT_OF_SPACE;
}

/*****************************************************************************
 * Run the test.
 *
 * First, clear the log in order in order to clear the OverflowFlag.  Then
 * fill up the Event Log.  Try adding a new entry.  It should fail.  Then
 * verify that it really wasn't added.  Lastly, verify that the OverflowFlag
 * was set.
 *****************************************************************************/

HpiTestStatus OverflowDrop::runAddTest(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.OverflowAction != SAHPI_EL_OVERFLOW_DROP) {
        status.assertNotSupport();
    } else {
        // must clear the OverflowFlag
        error = saHpiEventLogClear(sessionId, resourceId);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_CLEAR, SA_OK, error);
        } else  {
            status.add(TRACE, EventLogHelper::fill(sessionId, resourceId));
            if (status.isOkay()) {
                EventHelper::fill(&event, "OverflowDrop test");
                error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
                if (error != SA_ERR_HPI_OUT_OF_SPACE) {
                    status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD,
                                         SA_ERR_HPI_OUT_OF_SPACE, error);
                } else {
                    bool found;
                    status.add(TRACE, EventLogHelper::findEvent(sessionId, resourceId,
                                                                &event, &found));
                    if (status.isOkay() && found) {
                        status.assertFailure(TRACE, "The event was mistakenly added.");
                    }

                    error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
                    if (error != SA_OK) {
                        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
                    } else {
                        status.assertPass();
                        if (!info.OverflowFlag) {
                            status.assertFailure(TRACE, "OverflowFlag was not set.");
                        }
                        if (info.Entries > info.Size) {
                            status.assertFailure(TRACE, 
                                      "More entries than can fit into the Event Log.");
                        }
                    }
                }

                // since will filled up the Event Log, let's clear it for the following tests.
                error = saHpiEventLogClear(sessionId, resourceId);
                status.checkError(TRACE, EVENT_LOG_CLEAR, error);
            }
        }
    }

    return status;
}

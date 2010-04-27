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

#include "EmptyEventLog.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EmptyEventLog::EmptyEventLog(char *line) : EventLogTestCase(DISABLE_LOG, line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EmptyEventLog::getName() {
    return "EmptyEventLog";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *EmptyEventLog::getDescription() {
    return "The Event Log has no entries.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EmptyEventLog::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus EmptyEventLog::runLogTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    // The event log must be empty; so let's clear it

    SaErrorT error = saHpiEventLogClear(sessionId, resourceId);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_CLEAR, SA_OK, error);
    } else {
        error = saHpiEventLogEntryGet(sessionId, resourceId,
                                      SAHPI_OLDEST_ENTRY, 
                                      &prevEntryId, &nextEntryId,
                                      &eventLogEntry, NULL, NULL);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET, SA_ERR_HPI_NOT_PRESENT, error);
        }
    }

    return status;
}

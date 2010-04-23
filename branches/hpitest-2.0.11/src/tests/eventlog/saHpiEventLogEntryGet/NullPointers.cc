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

#include "NullPointers.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullPointers::NullPointers(char *line) : NonEmptyEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullPointers::getName() {
    return "NullPointers";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullPointers::getDescription() {
    return "Any of <i>PrevEntryId</i>, <i>NextEntryId</i>, and <i>EventLogEntry</i> "
           "pointers are passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullPointers::getPrecondition() {
    return "Requires an Event Log with entries or the ability to add entries.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullPointers::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullPointers::runNonEmptyTest(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    status.assertPass();

    SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId,
                                           SAHPI_OLDEST_ENTRY, 
                                           NULL, &nextEntryId,
                                           &eventLogEntry, NULL, NULL);
    if (error != SA_ERR_HPI_INVALID_PARAMS) {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error,
                             "PrevEntryId was set to NULL.");
    }

    error = saHpiEventLogEntryGet(sessionId, resourceId, SAHPI_OLDEST_ENTRY, 
                                  &prevEntryId, NULL, &eventLogEntry, NULL, NULL);
    if (error != SA_ERR_HPI_INVALID_PARAMS) {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error,
                             "NextEntryId was set to NULL.");
    }

    error = saHpiEventLogEntryGet(sessionId, resourceId, SAHPI_OLDEST_ENTRY, 
                                  &prevEntryId, &nextEntryId, NULL, NULL, NULL);
    if (error != SA_ERR_HPI_INVALID_PARAMS) {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error,
                             "EventLogEntry was set to NULL.");
    }

    return status;
}

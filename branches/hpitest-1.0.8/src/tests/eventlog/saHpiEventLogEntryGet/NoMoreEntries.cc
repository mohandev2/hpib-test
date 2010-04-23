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

#include "NoMoreEntries.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoMoreEntries::NoMoreEntries(char *line) : NonEmptyEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoMoreEntries::getName() {
    return "NoMoreEntries";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoMoreEntries::getDescription() {
    return "SAHPI_NO_MORE_ENTRIES is passed in to <i>EntryId</i>.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoMoreEntries::getPrecondition() {
    return "Requires a non-empty Event Log or the ability to add an "
           "entry to the Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoMoreEntries::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoMoreEntries::runNonEmptyTest(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId,
                                           SAHPI_NO_MORE_ENTRIES, 
                                           &prevEntryId, &nextEntryId,
                                           &eventLogEntry, NULL, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

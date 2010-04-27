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

#include "BackwardTraversal.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

BackwardTraversal::BackwardTraversal(char *line)
: NonEmptyEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *BackwardTraversal::getName() {
    return "BackwardTraversal";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *BackwardTraversal::getDescription() {
    return "Traverse the Event Log going backwards.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *BackwardTraversal::getPrecondition() {
    return "Requires an Event Log with entries or the ability to add entries.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT BackwardTraversal::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus BackwardTraversal::runNonEmptyTest(SaHpiSessionIdT sessionId,
                                                 SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryIdT entryId;
    SaHpiEventLogEntryT eventLogEntry;

    status.assertPass();
    prevEntryId = SAHPI_NEWEST_ENTRY;
    while (prevEntryId != SAHPI_NO_MORE_ENTRIES) {
        entryId = prevEntryId;
        SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId, entryId, 
                                               &prevEntryId, &nextEntryId,
                                               &eventLogEntry, NULL, NULL);
        if (error != SA_OK) {
            status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET, SA_OK, error);
            break;
        }
    }

    return status;
}

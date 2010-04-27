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

#include "EntryIdNotPresent.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EntryIdNotPresent::EntryIdNotPresent(char *line) : NonEmptyEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EntryIdNotPresent::getName() {
    return "EntryIdNotPresent";
}

/*****************************************************************************
 *Return the description of the test case.
 *****************************************************************************/

const char *EntryIdNotPresent::getDescription() {
    return "The entry identified by <i>EntryId</i> is not present.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *EntryIdNotPresent::getPrecondition() {
    return "Requires an Event Log with entries or the ability to add entries.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EntryIdNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus EntryIdNotPresent::runNonEmptyTest(SaHpiSessionIdT sessionId,
                                                 SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId,
                                           getInvalidEntryId(), 
                                           &prevEntryId, &nextEntryId,
                                           &eventLogEntry, NULL, NULL);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_GET,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

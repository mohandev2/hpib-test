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

#include "NoRdr.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoRdr::NoRdr(char *line) : NonEmptyEventLogTestCase(true, line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoRdr::getName() {
    return "NoRdr";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoRdr::getDescription() {
    return "If the Event Log does not include a logged RDR for the\n"
           "Event Log entry being returned, <i>Rdr->RdrType</i> will be set\n"
           "to SAHPI_NO_RECORD.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoRdr::getPrecondition() {
    return "Requires an Event Log with entries or the ability to add entries.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoRdr::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoRdr::runNonEmptyTest(SaHpiSessionIdT sessionId,
                                     SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryIdT entryId;
    SaHpiEventLogEntryT eventLogEntry;
    SaHpiRdrT rdr;

    status.assertPass();
    nextEntryId = SAHPI_OLDEST_ENTRY;
    while (nextEntryId != SAHPI_NO_MORE_ENTRIES) {
        entryId = nextEntryId;
        SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId, entryId, 
                                               &prevEntryId, &nextEntryId,
                                               &eventLogEntry, &rdr, NULL);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_ENTRY_GET, SA_OK, error);
            break;
        } else if ((eventLogEntry.Event.EventType == SAHPI_ET_USER) &&
                   (rdr.RdrType != SAHPI_NO_RECORD)) {
            status.assertFailure(TRACE, "User Event has an RDR record.");
            break;
        }
    }

    return status;
}

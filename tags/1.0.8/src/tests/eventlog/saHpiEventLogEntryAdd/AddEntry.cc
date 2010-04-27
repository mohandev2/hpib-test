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

#include "AddEntry.h"
#include "Report.h"
#include "EventHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddEntry::AddEntry(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddEntry::getName() {
    return "AddEntry";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddEntry::getDescription() {
    return "Add an event and verify that the event was added as the newest entry.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AddEntry::getPrecondition() {
    return "Requires that an Event Log entry be added without getting\n"
           "a SA_ERR_HPI_INVALID_DATA error code.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddEntry::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddEntry::runAddTest(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    Report report;
    SaHpiEventT event;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    EventHelper::fill(&event, "test");
    SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD, SA_OK, error);
    } else {
        error = saHpiEventLogEntryGet(sessionId, resourceId, SAHPI_NEWEST_ENTRY, 
                                      &prevEntryId, &nextEntryId, &eventLogEntry, NULL, NULL);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_ENTRY_GET, SA_OK, error);
        } else if (!EventHelper::isEqual(&event, &eventLogEntry.Event, report)) {
            status.assertFailure(TRACE, report.toString());
        } else {
            status.assertPass();
        }
    }

    return status;
}

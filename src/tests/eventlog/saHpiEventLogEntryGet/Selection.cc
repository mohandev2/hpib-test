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

#include "Selection.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Selection::Selection(char *line) : AddEventLogTestCase(3, line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Selection::getName() {
    return "Selection";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Selection::getDescription() {
    return "The special <i>EntryIds</i> SAHPI_OLDEST_ENTRY and SAHPI_NEWEST_ENTRY\n"
           "are used to select the oldest and newest entries, respectively,\n"
           "in the Event Log being read.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Selection::getPrecondition() {
    return "Requires the ability to add three event log entries "
           "to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Selection::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Clear the EventLog and then add three user events.  Verify that we can
 * read the first and third entries with SAHPI_NEWEST_ENTRY and
 * SAHPI_OLDEST_ENTRY.
 *****************************************************************************/

HpiTestStatus Selection::runAddTest(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event1;
    SaHpiEventT event2;
    SaHpiEventT event3;

    SaErrorT error = saHpiEventLogClear(sessionId, resourceId);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_CLEAR, SA_OK, error);
    } else {

        EventHelper::fill(&event1, "1"); // oldest
        EventHelper::fill(&event2, "2");
        EventHelper::fill(&event3, "3"); // newest
            
        status.add(TRACE, addEntries(sessionId, resourceId, &event1, &event2, &event3));
        if (status.isOkay()) {
            status.add(TRACE, verify(sessionId, resourceId, &event3, SAHPI_NEWEST_ENTRY, "newest"));
            status.add(TRACE, verify(sessionId, resourceId, &event1, SAHPI_OLDEST_ENTRY, "oldest"));
        }
    }

    return status;
}

/*****************************************************************************
 * Add the three events to the Event Log.
 *****************************************************************************/

HpiTestStatus Selection::addEntries(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId, 
                                    SaHpiEventT *event1,
                                    SaHpiEventT *event2,
                                    SaHpiEventT *event3) {

    HpiTestStatus status = EventLogHelper::addEntry(sessionId, resourceId, event1);
    if (status.isOkay()) {
        status = EventLogHelper::addEntry(sessionId, resourceId, event2);
        if (status.isOkay()) {
            status = EventLogHelper::addEntry(sessionId, resourceId, event3);
        }
    }

    return status;
}

/*****************************************************************************
 * Read the event as indicated by "entryId" and verify that it is the
 * same as "event".
 *****************************************************************************/

HpiTestStatus Selection::verify(SaHpiSessionIdT sessionId,
                                SaHpiResourceIdT resourceId, 
                                SaHpiEventT *event,
                                SaHpiEventLogEntryIdT entryId, const char *s) {

    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId, entryId, 
                                           &prevEntryId, &nextEntryId,
                                           &eventLogEntry, NULL, NULL);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_ENTRY_GET, SA_OK, error);
    } else if (!EventHelper::isEqual(event, &eventLogEntry.Event)) {
        status.assertFailure(TRACE, "Did not return the %s eventlog entry.", s);
    } else {
        status.assertPass();
    }

    return status;
}

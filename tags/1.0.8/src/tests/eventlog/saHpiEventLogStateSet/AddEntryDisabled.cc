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

#include "AddEntryDisabled.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddEntryDisabled::AddEntryDisabled(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddEntryDisabled::getName() {
    return "AddEntryDisabled";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddEntryDisabled::getDescription() {
    return "If the Event Log is disabled, events may still be added to "
           "the Event Log using saHpiEventLogEntryAdd().";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AddEntryDisabled::getPrecondition() {
    return "Requires the ability to add entries to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddEntryDisabled::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddEntryDisabled::runAddTest(SaHpiSessionIdT sessionId,
                                           SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;

    EventHelper::fill(&event, "State Set Test - Disabled");

    SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
    if (error != SA_OK) {
        status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD, SA_OK, error);
    } else {
        bool found;
        status.add(TRACE, EventLogHelper::findEvent(sessionId, resourceId,
                                                    &event, &found));
        if (status.isOkay()) {
            if (found) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                            "Failed to find the event in the EventLog.");
            }
        }
    }

    return status;
}

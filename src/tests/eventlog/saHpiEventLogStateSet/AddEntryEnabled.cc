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

#include "AddEntryEnabled.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddEntryEnabled::AddEntryEnabled(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 *  Return the name of the test case.
 *****************************************************************************/

const char *AddEntryEnabled::getName() {
    return "AddEntryEnabled";
}

/*****************************************************************************
 *  Return the description of the test case.
 *****************************************************************************/

const char *AddEntryEnabled::getDescription() {
    return "Add an event to an enabled Event Log.";
}

/*****************************************************************************
 *  Return the precondition of the test case.
 *****************************************************************************/

const char *AddEntryEnabled::getPrecondition() {
    return "Requires the ability to add entries to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddEntryEnabled::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddEntryEnabled::runAddTest(SaHpiSessionIdT sessionId,
                                          SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;

    EventHelper::fill(&event, "State Set Test - Enabled");

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
                            "Failed to find the event in the Event Log.");
            }
        }
    }

    return status;
}

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

#include "GenEventEnabled.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GenEventEnabled::GenEventEnabled(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GenEventEnabled::getName() {
    return "GenEventEnabled";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GenEventEnabled::getDescription() {
    return "If the Event Log is enabled, events generated within the HPI "
           "implementation will be added to the Event Log.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GenEventEnabled::getPrecondition() {
    return "Requires the following preconditions:\n"
           "<ol>\n"
           "<li>Must have available space in the Event Log or overwrite is enabled.</li>\n"
           "<li>Must be able to add an event via saHpiEventAdd().</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GenEventEnabled::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;
    SaHpiBoolT state;
    SaHpiEventT event;

    // Can an event be added to the Event Log?

    SaErrorT error = saHpiEventLogInfoGet(sessionId,
                                          SAHPI_UNSPECIFIED_RESOURCE_ID, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
    } else if (info.Size == 0) {
        status.assertNotSupport();
    } else if ((info.Entries == info.Size) && 
               (info.OverflowFlag != SAHPI_EL_OVERFLOW_OVERWRITE)) {
        status.assertNotSupport();
    } else {

        // Enable the Event Log if it isn't already enabled.

        error = saHpiEventLogStateGet(sessionId,
                                      SAHPI_UNSPECIFIED_RESOURCE_ID, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
        } else {
            if (!state) {
                error = saHpiEventLogStateSet(sessionId,
                                              SAHPI_UNSPECIFIED_RESOURCE_ID,
                                              SAHPI_TRUE);
                if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_LOG_STATE_SET, SA_OK, error);
                }
            }

            // Add an event into the HPI implementation and verify
            // that the event was added to the Event Log.

            if (status.isOkay()) {
                EventHelper::fill(&event, "Gen Event - Enabled");
                error = saHpiEventAdd(sessionId, &event);
                if (error == SA_ERR_HPI_INVALID_DATA) {
                    status.assertNotSupport();
                } else if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_ADD, SA_OK, error);
                } else {
                    pauseForEventProcessing();
                    
                    bool found;
                    status.add(TRACE, 
                            EventLogHelper::findEvent(sessionId,
                                                      SAHPI_UNSPECIFIED_RESOURCE_ID,
                                                      &event, &found));
                    if (status.isOkay()) {
                        if (found) {
                            status.assertPass();
                        } else {
                            status.assertPassWarn(TRACE, 
                                        "Did not find the generated event in the Domain Event Log, " 
                                        "but that is permitted according to the HPI specification.");
                        }
                    }
                }
            }

            // restore the state
            if (!state) {
                error = saHpiEventLogStateSet(sessionId, SAHPI_UNSPECIFIED_RESOURCE_ID, state);
                status.checkError(TRACE, EVENT_LOG_STATE_SET, error);
            }
        }
    }

    return status;
}

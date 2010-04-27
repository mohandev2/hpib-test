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

#include "GenEventDisabled.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GenEventDisabled::GenEventDisabled(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GenEventDisabled::getName() {
    return "GenEventDisabled";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GenEventDisabled::getDescription() {
    return "If the Event Log is disabled, no events generated within the "
           "HPI implementation will be added to the Event Log.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GenEventDisabled::getPrecondition() {
    return "Requires the following preconditions:"
           "<ol>"
           "<li>Must have available space in the Event Log or overwrite is enabled.</li>"
           "<li>Must be able to add an event via saHpiEventAdd().</li>"
           "</ol>";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GenEventDisabled::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;
    SaHpiBoolT state;
    SaHpiEventT event;

    // Check to see if we can add an event to the Event Log

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

        // Disable the Event Log, if it isn't already disabled.

        error = saHpiEventLogStateGet(sessionId,
                                      SAHPI_UNSPECIFIED_RESOURCE_ID, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
        } else {
            if (state) {
                error = saHpiEventLogStateSet(sessionId,
                                              SAHPI_UNSPECIFIED_RESOURCE_ID,
                                              SAHPI_FALSE);
                if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_LOG_STATE_SET, SA_OK, error);
                }
            }

            // Add an event to the HPI implementation and then check
            // to see if it showed up in the Event Log, which it shouldn't.

            if (status.isOkay()) {
                EventHelper::fill(&event, "Gen Event - Disabled");
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
                        if (!found) {
                            status.assertPass();
                        } else {
                            status.assertFailure(TRACE, "Generated event was added to "
                                                 "the disabled Domain Event Log.");
                        }
                    }
                }
            }

            // restore the state
            if (state) {
                error = saHpiEventLogStateSet(sessionId, SAHPI_UNSPECIFIED_RESOURCE_ID, state);
                status.checkError(TRACE, EVENT_LOG_STATE_SET, error);
            }
        }
    }

    return status;
}

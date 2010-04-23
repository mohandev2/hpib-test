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

#include "TestStatus.h"
#include "EventLogTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EventLogTestCase::EventLogTestCase(char *line) : HpiSessionTestCase(line) { 
    this->action = NULL_LOG;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EventLogTestCase::EventLogTestCase(int newAction, char *line) : HpiSessionTestCase(line) { 
    this->action = newAction;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus EventLogTestCase::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // test the domain event log
    status.merge(testResource(sessionId, SAHPI_UNSPECIFIED_RESOURCE_ID));

    // test each resource that supports event logs
    SaHpiRptEntryT rptEntry;
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (status.isOkay() && nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId,
                                          &nextEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else if (hasEventLogCapability(&rptEntry)) {
            pushLocation("Resource", rptEntry.ResourceId);
            status.merge(testResource(sessionId, rptEntry.ResourceId));
            popLocation();
        }
    }

    return status;
}

/*****************************************************************************
 * Depending upon the action, enable, disable, or do nothing with the eventlog
 * state.  Then run the test for the specific resource.
 *****************************************************************************/

HpiTestStatus EventLogTestCase::testResource(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    SaHpiBoolT state = SAHPI_FALSE;
    if (action == DISABLE_LOG || action == ENABLE_LOG) {
        SaErrorT error = saHpiEventLogStateGet(sessionId, resourceId, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
        } else if (action == DISABLE_LOG && state) {
            error = saHpiEventLogStateSet(sessionId, resourceId, SAHPI_FALSE);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_STATE_SET, SA_OK, error);
            }
        } else if (action == ENABLE_LOG && !state) {
            error = saHpiEventLogStateSet(sessionId, resourceId, SAHPI_TRUE);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_STATE_SET, SA_OK, error);
            }
        }
    }

    if (status.isOkay()) {
        status = runLogTest(sessionId, resourceId);
        if ((action == DISABLE_LOG && state) || (action == ENABLE_LOG && !state)) {

            SaErrorT error = saHpiEventLogStateSet(sessionId, resourceId, state);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_STATE_SET, SA_OK, error);
            }
        } 
    }

    return status;
}

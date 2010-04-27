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

#include "HotSwapActive.h"
#include "EventHelper.h"

using namespace ns_saHpiRptEntryGetByResourceId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HotSwapActive::HotSwapActive(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *HotSwapActive::getName() {
    return "HotSwapActive";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *HotSwapActive::getDescription() {
    return "Calls to saHpiRptEntryGetByResourceId() are valid in any\n"
           "hot swap state, except for SAHPI_HS_STATE_NOT_PRESENT.  This\n"
           "test will call saHpiRptEntryGetByResourceId() after a newly\n"
           "inserted FRU enters the SAHPI_HS_STATE_ACTIVE hot swap state.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT HotSwapActive::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * This is a manual test.
 *****************************************************************************/

bool HotSwapActive::isManual() {
    return true;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus HotSwapActive::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        status.add(TRACE, insertFRU(sessionId));

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Have the user insert an FRU.  Wait for hot swap event where the state
 * is ACTIVE.  Once that occurs, try calling saHpiRptEntryGetByResourceId
 * for that resource.  It should be okay.
 *****************************************************************************/

HpiTestStatus HotSwapActive::insertFRU(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiRptEntryT rptEntry;
    bool foundEvent;

    promptInsertFru(sessionId);

    EventMatcher matcher;
    matcher.setEventType(SAHPI_ET_HOTSWAP);
    matcher.setHotSwapState(SAHPI_HS_STATE_ACTIVE);
    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, &event, 
                                                NULL, &rptEntry, NULL, &foundEvent));
    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertError(TRACE, "Did not receive a HOT SWAP "
                                      "event in the ACTIVE state.");
        } else {
            SaErrorT error = saHpiRptEntryGetByResourceId(sessionId, 
                                                          rptEntry.ResourceId,
                                                          &rptEntry);
            if (error == SA_OK) { 
                status.assertPass();
            } else {
                status.assertFailure(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID,
                                     SA_OK, error);
            }
        }
    }

    return status;
}


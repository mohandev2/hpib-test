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

#include "HotSwapNotPresent.h"
#include "EventHelper.h"

using namespace ns_saHpiRptEntryGetByResourceId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HotSwapNotPresent::HotSwapNotPresent(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *HotSwapNotPresent::getName() {
    return "HotSwapNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *HotSwapNotPresent::getDescription() {
    return "Calls to saHpiRptEntryGetByResourceId() are valid in any\n"
           "hot swap state, except for SAHPI_HS_STATE_NOT_PRESENT.  This\n"
           "test will call saHpiRptEntryGetByResourceId() after an FRU\n"
           "is removed and the resource has entered the hot swap state\n"
           "SAHPI_HS_STATE_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT HotSwapNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_RESOURCE;
}

/*****************************************************************************
 * This is a manual test.
 *****************************************************************************/

bool HotSwapNotPresent::isManual() {
    return true;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus HotSwapNotPresent::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        status.add(TRACE, removeFRU(sessionId));

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Have the user remove an FRU.  Wait for a hot swap event where the 
 * resource has entered the NOT_PRESENT state.  Once that occurs, call
 * saHpiRptEntryGetByResourceId().  The call should fail.
 *****************************************************************************/

HpiTestStatus HotSwapNotPresent::removeFRU(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiRptEntryT rptEntry;
    bool foundEvent;

    promptRemoveFru(sessionId);

    EventMatcher matcher;
    matcher.setEventType(SAHPI_ET_HOTSWAP);
    matcher.addHotSwapState(SAHPI_HS_STATE_NOT_PRESENT);
    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, &event, 
                                                NULL, &rptEntry, NULL, &foundEvent));
    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertError(TRACE, "Did not receive a HOT SWAP "
                          "event in the NOT PRESENT state.");
        } else {
            SaErrorT error = saHpiRptEntryGetByResourceId(sessionId, 
                                                          rptEntry.ResourceId,
                                                          &rptEntry);
            if (error == SA_ERR_HPI_INVALID_RESOURCE) { 
                status.assertPass();
            } else {
                status.assertFailure(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, 
                                     SA_ERR_HPI_INVALID_RESOURCE, error);
            }
        }
    }

    return status;
}


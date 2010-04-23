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

#include "GetState.h"

using namespace ns_saHpiHotSwapStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetState::GetState(char *line) : HotSwapTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetState::getName() {
    return "GetState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetState::getDescription() {
    return "Retrieve the current hot swap state of a resource "
           "and verify that it is a legal state.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetState::getPrecondition() {
    return "Requires a managed hot swap resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetState::runHsTest(SaHpiSessionIdT sessionId,
                                  SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiHsStateT state;

    SaErrorT error = saHpiHotSwapStateGet(sessionId,
                                          rptEntry->ResourceId,
                                          &state);
    if (error != SA_OK) {
        status.assertFailure(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
    } else if (state == SAHPI_HS_STATE_NOT_PRESENT) {
        status.assertFailure(TRACE, 
                    "HotSwap State is SAHPI_HS_STATE_NOT_PRESENT.");
    } else if (!isValidState(state)) {
        status.assertFailure(TRACE, "Invalid HotSwap State: 0x%X.", state);
    } else {
        status.assertPass();
    }

    return status;
}

/*****************************************************************************
 * Is the state one of the valid enumerated values?
 *****************************************************************************/

bool GetState::isValidState(SaHpiHsStateT state) {
    return (state == SAHPI_HS_STATE_INACTIVE) ||
           (state == SAHPI_HS_STATE_ACTIVE) ||
           (state == SAHPI_HS_STATE_INSERTION_PENDING) ||
           (state == SAHPI_HS_STATE_EXTRACTION_PENDING);
}

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

using namespace ns_saHpiHotSwapIndicatorStateGet;

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
    return "Retrieve the current sate of teh hot swap indicator\n"
           "and verify that the value is legal.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetState::getPrecondition() {
    return "Requires a managed hot swap resource that supports a hot swap indicator.";
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
    SaHpiHsIndicatorStateT state;

    if (!isIndicatorSupported(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiHotSwapIndicatorStateGet(sessionId,
                                                       rptEntry->ResourceId,
                                                       &state);
        if (error != SA_OK) {
            status.assertFailure(TRACE, HOT_SWAP_INDICATOR_STATE_GET,
                                 SA_OK, error);
        } else if (!isValidState(state)) {
            status.assertFailure(TRACE, 
                        "Invalid HotSwap Indicator State: 0x%X.", state);
        } else {
            status.assertPass();
        }
    }

    return status;
}

/*****************************************************************************
 * Is this a valid state?
 *****************************************************************************/

bool GetState::isValidState(SaHpiHsIndicatorStateT state) {
    return (state == SAHPI_HS_INDICATOR_ON) ||
           (state == SAHPI_HS_INDICATOR_OFF);
}

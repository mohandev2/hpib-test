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

#include "SetState.h"

using namespace ns_saHpiHotSwapIndicatorStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetState::SetState(char *line) : HotSwapTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetState::getName() {
    return "SetState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetState::getDescription() {
    return "Toggle the state of the hot swap indicator.  After each set,\n"
           "retrieve the state to verify that the change was successful.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetState::getPrecondition() {
    return "Requires a managed hot swap resource that supports a hot swap indicator.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetState::runHsTest(SaHpiSessionIdT sessionId,
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
            status.assertError(TRACE, HOT_SWAP_INDICATOR_STATE_GET, SA_OK, error);
        } else {
            if (state == SAHPI_HS_INDICATOR_OFF) {
                status.add(TRACE, testIndicator(sessionId,
                                                rptEntry->ResourceId,
                                                SAHPI_HS_INDICATOR_ON));
                status.add(TRACE, testIndicator(sessionId,
                                                rptEntry->ResourceId,
                                                SAHPI_HS_INDICATOR_OFF));
            } else {
                status.add(TRACE, testIndicator(sessionId,
                                                rptEntry->ResourceId,
                                                SAHPI_HS_INDICATOR_OFF));
                status.add(TRACE, testIndicator(sessionId,
                                                rptEntry->ResourceId,
                                                SAHPI_HS_INDICATOR_ON));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Set the state of the hot swap indicator and verify that the
 * change was successful.
 *****************************************************************************/

HpiTestStatus SetState::testIndicator(SaHpiSessionIdT sessionId, 
                                      SaHpiResourceIdT resourceId, 
                                      SaHpiHsIndicatorStateT state) {
    HpiTestStatus status;
    SaHpiHsIndicatorStateT newState;

    SaErrorT error = saHpiHotSwapIndicatorStateSet(sessionId, resourceId, state);
    if (error != SA_OK) {
        status.assertFailure(TRACE, HOT_SWAP_INDICATOR_STATE_SET, SA_OK, error);
    } else {
        error = saHpiHotSwapIndicatorStateGet(sessionId, resourceId, &newState);
        if (error != SA_OK) {
            status.assertError(TRACE, HOT_SWAP_INDICATOR_STATE_GET, SA_OK, error);
        } else if (state == newState) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, 
                        "Failed to set the HotSwap Indicator state to %s.",
                        HpiString::hotSwapIndicatorState(state));
        }
    }

    return status;
}

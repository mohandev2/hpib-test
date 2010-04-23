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

#include "NullState.h"

using namespace ns_saHpiHotSwapStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullState::NullState(char *line) : HotSwapTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullState::getName() {
    return "NullState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullState::getDescription() {
    return "The <i>State</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullState::getPrecondition() {
    return "Requires a managed hot swap resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullState::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullState::runHsTest(SaHpiSessionIdT sessionId,
                                   SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    SaErrorT error = saHpiHotSwapStateGet(sessionId,
                                          rptEntry->ResourceId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, HOT_SWAP_STATE_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

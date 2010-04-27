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

#include "NotPending.h"

using namespace ns_saHpiHotSwapPolicyCancel;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NotPending::NotPending(char *line) : HotSwapTestCase(false, line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NotPending::getName() {
    return "NotPending";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NotPending::getDescription() {
    return "The resource is not in the INSERTION PENDING "
           "or EXTRACTION PENDING states.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NotPending::getPrecondition() {
    return "Requires a managed hot swap resource that is not in the "
           "INSERTION PENDING or EXTRACTION PENDING state.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NotPending::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NotPending::runHsTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiHsStateT state;

    SaErrorT error = saHpiHotSwapStateGet(sessionId,
                                          rptEntry->ResourceId,
                                          &state);
    if (error != SA_OK) {
        status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
    } else if ((state == SAHPI_HS_STATE_INSERTION_PENDING) || 
               (state == SAHPI_HS_STATE_EXTRACTION_PENDING)) {
        status.assertNotSupport();
    } else {
        error = saHpiHotSwapPolicyCancel(sessionId, rptEntry->ResourceId);
        if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, HOT_SWAP_POLICY_CANCEL,
                                 SA_ERR_HPI_INVALID_REQUEST, error);
        }
    }

    return status;
}

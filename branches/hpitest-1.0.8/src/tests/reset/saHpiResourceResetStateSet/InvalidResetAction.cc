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

#include "InvalidResetAction.h"

using namespace ns_saHpiResourceResetStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidResetAction::InvalidResetAction(char *line) : ResetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidResetAction::getName() {
    return "InvalidResetAction";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidResetAction::getDescription() {
    return "The <i>ResetAction</i> parameter is not set to a proper value.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidResetAction::getPrecondition() {
    return "Requires a resource that supports the RESET capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidResetAction::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidResetAction::runResetTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    int invalidResetAction = (int) SAHPI_RESET_DEASSERT + 1;

    SaErrorT error = saHpiResourceResetStateSet(sessionId, rptEntry->ResourceId, 
                                                (SaHpiResetActionT) invalidResetAction);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RESOURCE_RESET_STATE_SET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}

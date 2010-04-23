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

#include "NullResetAction.h"

using namespace ns_saHpiResourceResetStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullResetAction::NullResetAction(char *line) : ResetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullResetAction::getName() {
    return "NullResetAction";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullResetAction::getDescription() {
    return "The <i>ResetAction</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullResetAction::getPrecondition() {
    return "Requires a resource that supports the RESET capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullResetAction::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullResetAction::runResetTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    SaErrorT error = saHpiResourceResetStateGet(sessionId,
                                                rptEntry->ResourceId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RESOURCE_RESET_STATE_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

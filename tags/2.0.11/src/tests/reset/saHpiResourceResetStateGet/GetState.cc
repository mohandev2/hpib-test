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

using namespace ns_saHpiResourceResetStateGet;

/*****************************************************************************
 * GetState Test Case
 *****************************************************************************/

GetState::GetState(char *line) : ResetTestCase(line) {
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
    return "Retrieve the reset state of an entity identified in the\n"
           "resource's RPT entry.  Verify that the returned state\n"
           "is valid.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetState::getPrecondition() {
    return "Requires a resource that supports the RESET capability.";
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

HpiTestStatus GetState::runResetTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResetActionT resetAction;

    SaErrorT error = getResetState(sessionId, rptEntry->ResourceId, &resetAction);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_RESET_STATE_GET, SA_OK, error,
                             "Unable to get the ResetAction.");
    } else if (!isValidAction(resetAction)) {
        status.assertFailure(TRACE, "Invalid Reset Action: %s.", 
                             HpiString::resetAction(resetAction));
    } else {
        status.assertPass();
    }

    return status;
}

/*****************************************************************************
 * Is the reset action value valid or not?
 *****************************************************************************/

bool GetState::isValidAction(SaHpiResetActionT action) {
    return (action == SAHPI_RESET_ASSERT) ||
           (action == SAHPI_RESET_DEASSERT);
}

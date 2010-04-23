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

#include "ColdReset.h"

using namespace ns_saHpiResourceResetStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ColdReset::ColdReset(char *line) : ResetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ColdReset::getName() {
    return "ColdReset";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ColdReset::getDescription() {
    return "Perform a cold reset on an entity identified by the resource.\n"
           "After the cold reset, the entity's reset state must be DEASSERT.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ColdReset::getPrecondition() {
    return "Requires a resource that supports the RESET capability. The\n"
           "resource cannot be the resource the HPI User is running on.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ColdReset::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ColdReset::runResetTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResetActionT resetAction;
    SaHpiResourceIdT hpiResourceId;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    SaErrorT error = getHpiUserResource(sessionId, &hpiResourceId);
    if (error != SA_OK) {
        status.assertError(TRACE, RESOURCE_ID_GET, SA_OK, error);
    } else if (resourceId == hpiResourceId) {
        status.assertNotSupport();
    } else {
        error = setResetState(sessionId, resourceId, SAHPI_COLD_RESET);
        if (error == SA_ERR_HPI_INVALID_CMD) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, RESOURCE_RESET_STATE_SET, SA_OK, error,
                                 "Unable to perform a cold reset.");
        } else {
            pauseForColdReset();

            error = getResetState(sessionId, resourceId, &resetAction);
            if (error != SA_OK) {
                status.assertError(TRACE, RESOURCE_RESET_STATE_GET, SA_OK, error);
            } else if (resetAction == SAHPI_RESET_DEASSERT) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, "ResetAction is %s after a cold reset.", 
                                     HpiString::resetAction(resetAction));
            }
        }
    }

    return status;
}

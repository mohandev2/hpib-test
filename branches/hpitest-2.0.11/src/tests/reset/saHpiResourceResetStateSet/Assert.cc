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

#include "Assert.h"

using namespace ns_saHpiResourceResetStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Assert::Assert(char *line) : ResetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Assert::getName() {
    return "Assert";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Assert::getDescription() {
    return "Put an entity into a reset state by setting the state to ASSERT.\n"
           "Verify that the assert was successful.  Then bring the entity out\n"
           "of the reset state by setting the state to DEASSERT.  Verify that\n"
           "the deassert was successful.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Assert::getPrecondition() {
    return "Requires a resource that supports the RESET capability. The\n"
           "resource cannot be the resource the HPI User is running on and\n"
           "the resource must support the RESET ASSERT action.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Assert::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Assert::runResetTest(SaHpiSessionIdT sessionId,
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
        error = setResetState(sessionId, resourceId, SAHPI_RESET_ASSERT);
        if (error == SA_ERR_HPI_INVALID_CMD) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, RESOURCE_RESET_STATE_SET, SA_OK, error, 
                                  "Unable to perform a reset assert.");
        } else {
            error = getResetState(sessionId, resourceId, &resetAction);
            if (error != SA_OK) {
                status.assertError(TRACE, RESOURCE_RESET_STATE_GET, SA_OK, error);
            } else if (resetAction != SAHPI_RESET_ASSERT) {
                status.assertFailure(TRACE, 
                            "ResetAction is %s after a reset assertion.", 
                            HpiString::resetAction(resetAction));
            } else {
                error = setResetState(sessionId, resourceId, SAHPI_RESET_DEASSERT);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, RESOURCE_RESET_STATE_SET, SA_OK, error,
                                          "Unable to perform a reset deassert.");
                } else {
                    error = getResetState(sessionId, resourceId, &resetAction);
                    if (error != SA_OK) {
                        status.assertError(TRACE, RESOURCE_RESET_STATE_GET, SA_OK, error);
                    } else if (resetAction == SAHPI_RESET_DEASSERT) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, 
                                        "ResetAction is %s after a reset deassertion.", 
                                        HpiString::resetAction(resetAction));
                    }
                }
            }
        }
    }

    return status;
}

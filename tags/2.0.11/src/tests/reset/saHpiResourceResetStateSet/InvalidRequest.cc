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

#include "InvalidRequest.h"

using namespace ns_saHpiResourceResetStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidRequest::InvalidRequest(char *line) : ResetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidRequest::getName() {
    return "InvalidRequest";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidRequest::getDescription() {
    return "The entity's reset state is currently asserted and the\n"
           "<i>ResetAction</i> parameter is SAHPI_COLD_RESET or\n"
           "SAHPI_WARM_RESET.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidRequest::getPrecondition() {
    return "Requires a resource that supports the RESET capability. The\n"
           "resource cannot be the resource the HPI User is running on and\n"
           "the resource must support the RESET ASSERT action.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidRequest::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidRequest::runResetTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
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
            status.assertPass();

            error = saHpiResourceResetStateSet(sessionId, resourceId, SAHPI_COLD_RESET);
            if (error != SA_ERR_HPI_INVALID_REQUEST) {
                status.assertFailure(TRACE, RESOURCE_RESET_STATE_SET,
                                      SA_ERR_HPI_INVALID_REQUEST, error);
            }

            error = saHpiResourceResetStateSet(sessionId, resourceId, SAHPI_WARM_RESET);
            if (error != SA_ERR_HPI_INVALID_REQUEST) {
                status.assertFailure(TRACE, RESOURCE_RESET_STATE_SET,
                                      SA_ERR_HPI_INVALID_REQUEST, error);
            }

            error = setResetState(sessionId, resourceId, SAHPI_RESET_DEASSERT);
            status.checkError(TRACE, RESOURCE_RESET_STATE_GET, error);
        }
    }

    return status;
}

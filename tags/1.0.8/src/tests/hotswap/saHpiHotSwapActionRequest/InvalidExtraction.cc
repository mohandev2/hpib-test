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

#include "InvalidExtraction.h"

using namespace ns_saHpiHotSwapActionRequest;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidExtraction::InvalidExtraction(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidExtraction::getName() {
    return "InvalidExtraction";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidExtraction::getDescription() {
    return "The resource is not in an appropriate hot swap state.  In this\n"
           "case, the resource is not in an ACTIVE state and the requested\n"
           "action is an extraction.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidExtraction::getPrecondition() {
    return "Requires a managed hot swap resource that is not being used by the HPI User.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidExtraction::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidExtraction::runHsTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (hsIsHpiResource) {
        status.assertNotSupport();
    } else {
        if (hsState != SAHPI_HS_STATE_ACTIVE) {
            status.add(TRACE, testResource(sessionId, resourceId));
        } else { // ACTIVE
            SaErrorT error = saHpiSubscribe(sessionId);
            if (error != SA_OK) {
                status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
            } else {
                status.add(TRACE, extractResource(sessionId, resourceId));
                if (status.isOkay()) {
                    status.add(TRACE, testResource(sessionId, resourceId));
                }
                status.add(TRACE, insertResource(sessionId, resourceId));

                error = saHpiUnsubscribe(sessionId);
                status.checkError(TRACE, UNSUBSCRIBE, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test doing an extraction on a resource that is not in the active state.
 *****************************************************************************/

HpiTestStatus InvalidExtraction::testResource(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId, 
                                               SAHPI_HS_ACTION_EXTRACTION);
    if (error == SA_ERR_HPI_INVALID_REQUEST) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, HOT_SWAP_ACTION_REQUEST,
                             SA_ERR_HPI_INVALID_REQUEST, error);
    }

    return status;
}

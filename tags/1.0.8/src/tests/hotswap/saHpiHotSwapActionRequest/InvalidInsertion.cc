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

#include "InvalidInsertion.h"

using namespace ns_saHpiHotSwapActionRequest;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidInsertion::InvalidInsertion(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidInsertion::getName() {
    return "InvalidInsertion";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidInsertion::getDescription() {
    return "The resource is not in an appropriate hot swap state.  In this\n"
           "case, the resource is not in the INACTIVE state and the requested\n"
           "action is an insertion.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidInsertion::getPrecondition() {
    return "Requires a managed hot swap resource that is not being used by the HPI User.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidInsertion::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidInsertion::runHsTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (hsIsHpiResource) {
        status.assertNotSupport();
    } else {
        if (hsState != SAHPI_HS_STATE_INACTIVE) {
            status.add(TRACE, testResource(sessionId, resourceId));
        } else { // INACTIVE
            SaErrorT error = saHpiSubscribe(sessionId);
            if (error != SA_OK) {
                status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
            } else {
                status.add(TRACE, insertResource(sessionId, resourceId));
                if (status.isOkay()) {
                    status.add(TRACE, testResource(sessionId, resourceId));
                }
                status.add(TRACE, extractResource(sessionId, resourceId));

                error = saHpiUnsubscribe(sessionId);
                status.checkError(TRACE, UNSUBSCRIBE, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test doing an insertion on a resource that is not in the inactive state.
 *****************************************************************************/

HpiTestStatus InvalidInsertion::testResource(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId, 
                                               SAHPI_HS_ACTION_INSERTION);
    if (error == SA_ERR_HPI_INVALID_REQUEST) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, HOT_SWAP_ACTION_REQUEST,
                             SA_ERR_HPI_INVALID_REQUEST, error);
    }

    return status;
}

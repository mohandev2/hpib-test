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

#include "Extract.h"

using namespace ns_saHpiResourceInactiveSet;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define TIMEOUT 5000 // msec

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Extract::Extract(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Extract::getName() {
    return "Extract";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Extract::getDescription() {
    return "Place a resource into the EXTRACTION PENDING state and then\n"
           "call this function to force the resource to transition to the\n"
           "INACTIVE state.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Extract::getPrecondition() {
    return "The following preconditions are required:\n"
           "<ol>\n"
           "<li>A resource that the HPI User is not running on.</li>\n"
           "<li>A resource in the ACTIVE or INACTIVE state.</li>\n"
           "<li>An extraction timeout that can be changed or is at least 2 seconds.</li>\n"
           "<li>An insertion timeout that is not BLOCK.</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Extract::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Check the preconditions and run the test.
 *****************************************************************************/

HpiTestStatus Extract::runHsTest(SaHpiSessionIdT sessionId,
                                 SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (hsIsHpiResource) {
        status.assertNotSupport();
    } else if (hsState != SAHPI_HS_STATE_ACTIVE && hsState != SAHPI_HS_STATE_INACTIVE) {
        status.assertNotSupport();
    } else if (hsAutoExtractReadOnly && 
               hsAutoExtractTimeout < AUTO_TIMEOUT && 
               hsAutoExtractTimeout != SAHPI_TIMEOUT_BLOCK) {
        status.assertNotSupport();
    } else if (hsAutoInsertTimeout == SAHPI_TIMEOUT_BLOCK) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSubscribe(sessionId);
        if (error != SA_OK) {
            status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
        } else {
            if (!hsAutoExtractReadOnly) {
                error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, AUTO_TIMEOUT);
                if (error != SA_OK) {
                    status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, SA_OK, error);
                }
            }

            if (status.isOkay()) {
                if (hsState == SAHPI_HS_STATE_ACTIVE) {
                    status.add(TRACE, testResource(sessionId, resourceId));

                    // restore resource to original active state
                    status.add(TRACE, insertResource(sessionId, resourceId));
                } else {
                    status.add(TRACE, insertResource(sessionId, resourceId));
                    if (status.isOkay()) {
                        status.add(TRACE, testResource(sessionId, resourceId));
                    }
                    // make sure we restore resource to original state
                    status.add(TRACE, extractResource(sessionId, resourceId));
                }

                status.add(TRACE, restoreTimeouts(sessionId, resourceId));
            }

            error = saHpiUnsubscribe(sessionId);
            status.checkError(TRACE, UNSUBSCRIBE, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Test an resource by doing a software extraction and then canceling the
 * extraction policy.  At this point we can continue the extraction by setting
 * the resource to inactive.
 *****************************************************************************/

HpiTestStatus Extract::testResource(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiHsStateT state;

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId,
                                               SAHPI_HS_ACTION_EXTRACTION);
    if (error != SA_OK) {
        status.assertError(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
    } else {
        status.add(TRACE, waitForHsEvent(sessionId, resourceId,
                                         SAHPI_HS_STATE_EXTRACTION_PENDING, TIMEOUT));
        if (status.isOkay()) {
            error = saHpiHotSwapPolicyCancel(sessionId, resourceId);
            if (error != SA_OK) {
                status.assertError(TRACE, HOT_SWAP_POLICY_CANCEL, SA_OK, error);
            } else {
                error = saHpiResourceInactiveSet(sessionId, resourceId);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, RESOURCE_INACTIVE_SET, SA_OK, error);
                } else {
                    pauseForAutoExtractionPolicy();
                    error = saHpiHotSwapStateGet(sessionId, resourceId, &state);
                    if (error != SA_OK) {
                        status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
                    } else if (state == SAHPI_HS_STATE_INACTIVE) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, "The state is expected to be INACTIVE but it is %s",
                                             HpiString::hotSwapState(state));
                    }
                }
            }
        }
    }

    return status;
}

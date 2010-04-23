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

using namespace ns_saHpiResourceActiveSet;

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
    return "An HPI User may use this function to request a resource to\n"
           "return to the ACTIVE state from the EXTRACTION PENDING state\n"
           "in order to reject an extraction request.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Extract::getPrecondition() {
    return "The following preconditions are required:\n"
           "<ol>\n"
           "<li>A resource that the HPI User is not running on.</li>\n"
           "<li>A resource in either the ACTIVE or INACTIVE state.</li>\n"
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
            // If possible, set the timeout to 2 seconds.  Because of the preconditions,
            // the timeout will be at least 2 seconds if we can't set it.  Two seconds
            // should be plenty of time to set the resource back to the ACTIVE state.

            if (!hsAutoExtractReadOnly) {
                error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, AUTO_TIMEOUT);
                if (error != SA_OK) {
                    status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, SA_OK, error);
                }
            }

            if (status.isOkay()) {
                if (hsState == SAHPI_HS_STATE_ACTIVE) {
                    status.add(TRACE, testResource(sessionId, resourceId));
                } else { // INACTIVE
                    status.add(TRACE, insertResource(sessionId, resourceId));
                    if (status.isOkay()) {
                        status.add(TRACE, testResource(sessionId, resourceId));
                    }
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
 * Test an active resource by doing a software extraction and then rejecting
 * the extraction by returning to the Active state.
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
            error = saHpiResourceActiveSet(sessionId, resourceId);
            if (error != SA_OK) {
               status.assertFailure(TRACE, RESOURCE_ACTIVE_SET, SA_OK, error);
            } else {
                pauseForAutoInsertionPolicy();
                error = saHpiHotSwapStateGet(sessionId, resourceId, &state);
                if (error != SA_OK) {
                    status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
                } else if (state == SAHPI_HS_STATE_ACTIVE) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, "The state is expected to be ACTIVE but it is %s",
                                         HpiString::hotSwapState(state));
                }
            }
        }
    }

    return status;
}

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

#include "PolicyCancel.h"

using namespace ns_saHpiHotSwapPolicyCancel;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PolicyCancel::PolicyCancel(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *PolicyCancel::getName() {
    return "PolicyCancel";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *PolicyCancel::getDescription() {
    return "Cancel the default policy during INSERTION PENDING "
           "and EXTRACTION PENDING.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *PolicyCancel::getPrecondition() {
    return "The following preconditions must be met:\n"
           "<ol>\n"
           "<li>The resource cannot be the same resource the HPI User is running on.</li>\n"
           "<li>The resource is in the ACTIVE or INACTIVE state.</li>\n"
           "<li>The Hot Swap Timeouts can be changed or are at least two seconds and not blocking.</li>\n"
           "</ol>\n";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT PolicyCancel::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Check the preconditions.
 *****************************************************************************/

HpiTestStatus PolicyCancel::runHsTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (hsIsHpiResource) {
        status.assertNotSupport();
    } else if ((hsState != SAHPI_HS_STATE_ACTIVE) && (hsState != SAHPI_HS_STATE_INACTIVE)) {
        status.assertNotSupport();
    } else if (hsAutoInsertReadOnly && hsAutoInsertTimeout == SAHPI_TIMEOUT_BLOCK) {
        status.assertNotSupport();
    } else if (hsAutoExtractReadOnly && hsAutoExtractTimeout == SAHPI_TIMEOUT_BLOCK) {
        status.assertNotSupport();
    } else if (hsAutoInsertReadOnly && hsAutoInsertTimeout < AUTO_TIMEOUT) {
        status.assertNotSupport();
    } else if (hsAutoExtractReadOnly && hsAutoExtractTimeout < AUTO_TIMEOUT) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, testResource(sessionId, resourceId));
    }

    return status;
}

/*****************************************************************************
 * Set the timeouts if possible and then run the insertion/extraction
 * tests to verify the operation of the cancel policy.
 *****************************************************************************/

HpiTestStatus PolicyCancel::testResource(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT resourceId) {

    HpiTestStatus status;

    SaHpiTimeoutT autoInsertionTimeout = hsAutoInsertTimeout;
    SaHpiTimeoutT autoExtractionTimeout = hsAutoExtractTimeout;

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        // Set the insert timeout to 2 seconds if we can; that should give
        // us more than enough time to cancel the policy.  If we can't set
        // it, the precondition guarantees is at least 2 seconds.

        if (!hsAutoInsertReadOnly) {
            autoInsertionTimeout = AUTO_TIMEOUT;
            error = saHpiAutoInsertTimeoutSet(sessionId, AUTO_TIMEOUT);
            if (error != SA_OK) {
                status.assertError(TRACE, AUTO_INSERT_TIMEOUT_SET, SA_OK, error);
            }
        }

        // Set the extract timeout to 2 seconds if we can; that should give
        // us more than enough time to cancel the policy.  If we can't set
        // it, the precondition guarantees is at least 2 seconds.

        if (status.isOkay()) {
            if (!hsAutoExtractReadOnly) {
                autoExtractionTimeout = AUTO_TIMEOUT;
                error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, AUTO_TIMEOUT);
                if (error != SA_OK) {
                    status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, SA_OK, error);
                }
            }
        }

        if (status.isOkay()) {
            if (hsState == SAHPI_HS_STATE_ACTIVE) {
                status.add(TRACE, extractTest(sessionId, resourceId, autoExtractionTimeout));
                if (status.isOkay()) {
                    status.add(TRACE, insertTest(sessionId, resourceId, autoInsertionTimeout));
                } else { // try to restore the original state
                    status.add(TRACE, insertResource(sessionId, resourceId));
                }
            } else { // INACTIVE
                status.add(TRACE, insertTest(sessionId, resourceId, autoInsertionTimeout));
                if (status.isOkay()) {
                    status.add(TRACE, extractTest(sessionId, resourceId, autoExtractionTimeout));
                } else { // try to restore the original state
                    status.add(TRACE, extractResource(sessionId, resourceId));
                }
            }
        }

        status.add(TRACE, restoreTimeouts(sessionId, resourceId));

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Verify that a cancel policy works for an extraction.
 *****************************************************************************/

HpiTestStatus PolicyCancel::extractTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiTimeoutT timeout) {
    HpiTestStatus status;
    SaHpiHsStateT state;

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId,
                                               SAHPI_HS_ACTION_EXTRACTION);
    if (error != SA_OK) {
        status.assertError(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
    } else {

        // When the resource enters the PENDING state, we can cancel the policy.

        TimeMsec msec = (timeout / 1000000) + getAutoExtractionPolicyTime();
        status.add(TRACE, waitForHsEvent(sessionId, resourceId, SAHPI_HS_STATE_EXTRACTION_PENDING, msec));
        if (status.isOkay()) {
            error = saHpiHotSwapPolicyCancel(sessionId, resourceId);
            if (error != SA_OK) {
                status.assertError(TRACE, HOT_SWAP_POLICY_CANCEL, SA_OK, error);
            } else {

                // Wait for the timeout and the time for the extraction policy. 
                // We need to wait to verify that the cancel worked, i.e. that 
                // the resource stayed in the PENDING state.

                long seconds = timeout / 1000000000;
                secSleep(seconds + 1);
                pauseForAutoExtractionPolicy();

                error = saHpiHotSwapStateGet(sessionId, resourceId, &state);
                if (error != SA_OK) {
                    status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
                } else if (state != SAHPI_HS_STATE_EXTRACTION_PENDING) {
                    status.assertFailure(TRACE, 
                                         "Policy Cancel failed; state is %s instead of EXTRACTION_PENDING.",
                                         HpiString::hotSwapState(state));
                } else {
                    status.assertPass();

                    // finish putting the resource in the INACTIVE state
                    status.add(TRACE, extractResource(sessionId, resourceId));
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Verify that a cancel policy works for an insertion.
 *****************************************************************************/

HpiTestStatus PolicyCancel::insertTest(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId,
                                       SaHpiTimeoutT timeout) {
    HpiTestStatus status;
    SaHpiHsStateT state;

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId,
                                               SAHPI_HS_ACTION_INSERTION);
    if (error != SA_OK) {
        status.assertError(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
    } else {

        // When the resource enters the PENDING state, we can cancel the policy.

        TimeMsec msec = (timeout / 1000000) + getAutoInsertionPolicyTime();
        status.add(TRACE, waitForHsEvent(sessionId, resourceId, SAHPI_HS_STATE_INSERTION_PENDING, msec));
        if (status.isOkay()) {
            error = saHpiHotSwapPolicyCancel(sessionId, resourceId);
            if (error != SA_OK) {
                status.assertError(TRACE, HOT_SWAP_POLICY_CANCEL, SA_OK, error);
            } else {

                // Wait for the timeout and the time for the insertion policy. 
                // We need to wait to verify that the cancel worked, i.e. that 
                // the resource stayed in the PENDING state.

                long seconds = timeout / 1000000000;
                secSleep(seconds + 1);
                pauseForAutoInsertionPolicy();

                error = saHpiHotSwapStateGet(sessionId, resourceId, &state);
                if (error != SA_OK) {
                    status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
                } else if (state != SAHPI_HS_STATE_INSERTION_PENDING) {
                    status.assertFailure(TRACE,
                                         "Policy Cancel failed; state is %s instead of INSERTION_PENDING.",
                                         HpiString::hotSwapState(state));
                } else {
                    status.assertPass();

                    // finish putting the resource in the ACTIVE state
                    status.add(TRACE, insertResource(sessionId, resourceId));
                }
            }
        }
    }

    return status;
}



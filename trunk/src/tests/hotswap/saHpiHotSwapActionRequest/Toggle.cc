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

#include "Toggle.h"
#include "EventMatcher.h"
#include "EventHelper.h"

using namespace ns_saHpiHotSwapActionRequest;

/*****************************************************************************
 * Constants
 *****************************************************************************/

// Get total time for insertion/extraction plus one second to avoid
// boundary conditions.  The timeout is in nanoseconds while the policy
// time is in msec.  Compute in msec time.
#define HS_TIME(timeout, policyTime) (timeout / 1000000 + policyTime + 1000)

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Toggle::Toggle(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Toggle::getName() {
    return "Toggle";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Toggle::getDescription() {
    return "The FRU's hot swap state is toggled and then restored.  If the\n"
           "the FRU's hot swap state is currently ACTIVE, the test will\n"
           "use the following steps:\n"
           "<ol>\n"
           "<li>Request a software extraction.</li>\n"
           "<li>Wait for an EXTRACTION PENDING hot swap event.</li>\n"
           "<li>Wait for an INACTIVE hot swap event.</li>\n"
           "<li>Request a software insertion.</li>\n"
           "<li>Wait for an INSERTION PENDING hot swap event.</li>\n"
           "<li>Wait for an ACTIVE hot swap event.</li>\n"
           "</ol>\n"
           "For a FRU that is currently in the INACTIVE state, the opposite\n"
           "of the above steps is done.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Toggle::getPrecondition() {
    return "The following preconditions must be met:\n"
           "<ol>\n"
           "<li>The resource cannot be the same resource the HPI User is running on.</li>\n"
           "<li>The resource is in the ACTIVE or INACTIVE state.</li>\n"
           "<li>The Hot Swap Timeouts are not set to BLOCK or can be changed.</li>\n"
           "</ol>\n";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Toggle::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Check the preconditions.
 *****************************************************************************/

HpiTestStatus Toggle::runHsTest(SaHpiSessionIdT sessionId,
                                SaHpiRptEntryT *rptEntry) {

    HpiTestStatus status;

    if (hsIsHpiResource) {
        status.assertNotSupport();
    } else if ((hsState != SAHPI_HS_STATE_ACTIVE) && (hsState != SAHPI_HS_STATE_INACTIVE)) {
        status.assertNotSupport();
    } else if (hsAutoInsertReadOnly && hsAutoInsertTimeout == SAHPI_TIMEOUT_BLOCK) {
        status.assertNotSupport();
    } else if (hsAutoExtractReadOnly && hsAutoExtractTimeout == SAHPI_TIMEOUT_BLOCK) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, runToggleTest(sessionId, rptEntry));
    }

    return status;
}

/*****************************************************************************
 * If possible, change the timeouts to speed up the tests.
 *****************************************************************************/

HpiTestStatus Toggle::runToggleTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    SaHpiTimeoutT autoInsertTimeout = hsAutoInsertTimeout;
    SaHpiTimeoutT autoExtractTimeout = hsAutoExtractTimeout;

    if (!hsAutoInsertReadOnly) {
        autoInsertTimeout = SAHPI_TIMEOUT_IMMEDIATE;
        SaErrorT error = saHpiAutoInsertTimeoutSet(sessionId, SAHPI_TIMEOUT_IMMEDIATE);
        if (error != SA_OK) {
            status.assertError(TRACE, AUTO_INSERT_TIMEOUT_SET, SA_OK, error);
        }
    }

    if (status.isOkay()) {
        if (!hsAutoExtractReadOnly) {
            autoExtractTimeout = SAHPI_TIMEOUT_IMMEDIATE;
            SaErrorT error = 
                saHpiAutoExtractTimeoutSet(sessionId, resourceId, SAHPI_TIMEOUT_IMMEDIATE);
            if (error != SA_OK) {
                status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, SA_OK, error);
            }
        }
    }

    if (status.isOkay()) {
        status.add(TRACE, runActionTest(sessionId, resourceId, 
                                        autoInsertTimeout,
                                        autoExtractTimeout));
    }

    status.add(TRACE, restoreTimeouts(sessionId, resourceId));

    return status;
}

/*****************************************************************************
 * Subscribe for events and then do the toggling of insertion/extraction.
 *****************************************************************************/

HpiTestStatus Toggle::runActionTest(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId,
                                    SaHpiTimeoutT autoInsertTimeout,
                                    SaHpiTimeoutT autoExtractTimeout) {
    HpiTestStatus status;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        if (hsState == SAHPI_HS_STATE_ACTIVE) {
            status.add(TRACE, extractInsertTest(sessionId, resourceId,
                                                autoInsertTimeout,
                                                autoExtractTimeout));
        } else if (hsState == SAHPI_HS_STATE_INACTIVE) {
            status.add(TRACE, insertExtractTest(sessionId, resourceId,
                                                autoInsertTimeout,
                                                autoExtractTimeout));
        } else {
            status.assertNotSupport();
	}

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Toggle a resource by doing an insertion followed by an extraction.
 *****************************************************************************/

HpiTestStatus Toggle::insertExtractTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiTimeoutT autoInsertTimeout,
                                        SaHpiTimeoutT autoExtractTimeout) {
    HpiTestStatus status;

    // Do a software insertion.

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId, SAHPI_HS_ACTION_INSERTION);
    if (error != SA_OK) {
        status.assertFailure(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
    } else {

        // Let's asssume that all will go well.
        status.assertPass();

        // Pause for the insertion to complete and then wait look for
        // the insertion pending and active event states.

        TimeMsec time = HS_TIME(autoInsertTimeout, getAutoInsertionPolicyTime());
        status.add(TRACE, waitForEvents(sessionId, resourceId,
                                        SAHPI_HS_STATE_INSERTION_PENDING,
                                        SAHPI_HS_STATE_ACTIVE, time));

        // Even if I didn't get the expected events, I will continue
        // because I should try to restore the original state of the resource.

        error = saHpiHotSwapActionRequest(sessionId, resourceId, SAHPI_HS_ACTION_EXTRACTION);
        if (error != SA_OK) {
            status.assertFailure(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
        } else {
            // Pause for the extraction to complete and then wait look for
            // the extraction pending and inactive event states.

            time = HS_TIME(autoExtractTimeout, getAutoExtractionPolicyTime());
            status.add(TRACE, waitForEvents(sessionId, resourceId,
                                            SAHPI_HS_STATE_EXTRACTION_PENDING,
                                            SAHPI_HS_STATE_INACTIVE, time));
        }
    }

    return status;
}

/*****************************************************************************
 * Toggle a resource by doing an extraction followed by an insertion.
 *****************************************************************************/

HpiTestStatus Toggle::extractInsertTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiTimeoutT autoInsertTimeout,
                                        SaHpiTimeoutT autoExtractTimeout) {
    HpiTestStatus status;

    // Do a software extraction.

    SaErrorT error = saHpiHotSwapActionRequest(sessionId, resourceId, SAHPI_HS_ACTION_EXTRACTION);
    if (error != SA_OK) {
        status.assertFailure(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
    } else {

        // Let's asssume that all will go well.
        status.assertPass();

        TimeMsec time = HS_TIME(autoExtractTimeout, getAutoExtractionPolicyTime());
        status.add(TRACE, waitForEvents(sessionId, resourceId,
                                        SAHPI_HS_STATE_EXTRACTION_PENDING,
                                        SAHPI_HS_STATE_INACTIVE, time));


        error = saHpiHotSwapActionRequest(sessionId, resourceId, SAHPI_HS_ACTION_INSERTION);
        if (error != SA_OK) {
            status.assertFailure(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
        } else {
            // Pause for the insertion to complete and then wait look for
            // the insertion pending and active event states.

            time = HS_TIME(autoInsertTimeout, getAutoInsertionPolicyTime());
            status.add(TRACE, waitForEvents(sessionId, resourceId,
                                            SAHPI_HS_STATE_INSERTION_PENDING,
                                            SAHPI_HS_STATE_ACTIVE, time));
        }
    }

    return status;
}

/*****************************************************************************
 * Wait for the incoming events due to an insertion or extraction.  First,
 * we should get "state1" which is a pending event followed by "state2" which
 * is either the ACTIVE or INACTIVE state.
 *
 * The "time" determines how long to wait for the second event.  The
 * time for a insertion/extraction depends upon the hot swap timeout and
 * the time to execute the policy.
 *****************************************************************************/
 
HpiTestStatus Toggle::waitForEvents(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId,
                                    SaHpiHsStateT state1,
                                    SaHpiHsStateT state2,
                                    TimeMsec time) {
    HpiTestStatus status;
    SaHpiEventT event;
    bool foundEvent;
    EventMatcher matcher;

    matcher.setSource(resourceId);
    matcher.setEventType(SAHPI_ET_HOTSWAP);
    matcher.setHotSwapState(state1);

    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher,
                                                &event, NULL, NULL, NULL,
                                                &foundEvent));

    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertFailure(TRACE, "Did not receive %s hot swap event.",
                                 HpiString::hotSwapState(state1));
        } else {
            matcher.setHotSwapState(state2);
            status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, time,
                                                        &event, NULL, NULL, NULL,
                                                        &foundEvent));

            if (status.isOkay() && !foundEvent) {
                status.assertFailure(TRACE, "Did not receive %s hot swap event.",
                             HpiString::hotSwapState(state2));
            }
        }
    }

    return status;
}

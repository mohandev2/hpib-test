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

#include "HotSwapTestCase.h"
#include "EventHelper.h"
#include "EventMatcher.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HotSwapTestCase::HotSwapTestCase() : HpiResourceTestCase() {
    this->gatherInfo = false;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HotSwapTestCase::HotSwapTestCase(char *line) : HpiResourceTestCase(line) {
    this->gatherInfo = false;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HotSwapTestCase::HotSwapTestCase(char *line, bool gather) : HpiResourceTestCase(line) {
    this->gatherInfo = gather;
}

/*****************************************************************************
 * Return the test timeout duration in seconds.
 *****************************************************************************/

int HotSwapTestCase::getTimeout() {
    return getHsTestTimeout();
}

/*****************************************************************************
 * Must have the FRU and Managed HotSwap capabilities in order to test.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::runResourceTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasFruCapability(rptEntry)) {
        status.assertNotSupport();
    } else if (!hasManagedHotSwapCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, gatherHsInfo(sessionId, rptEntry));
        if (status.isOkay()) {
            status = runHsTest(sessionId, rptEntry);
        }
    }

    return status;
}

/*****************************************************************************
 * Gather some general information that is needed by many of the tests.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::gatherHsInfo(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;
    SaHpiResourceIdT hpiResourceId;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (gatherInfo) {
        hsAutoExtractReadOnly = 
            rptEntry->HotSwapCapabilities & SAHPI_HS_CAPABILITY_AUTOEXTRACT_READ_ONLY;

        hsAutoInsertReadOnly = false;
        SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
        if (error != SA_OK) {
            status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
        } else {
            if (domainInfo.DomainCapabilities & SAHPI_DOMAIN_CAP_AUTOINSERT_READ_ONLY) {
                hsAutoInsertReadOnly = true;
            }

            error = saHpiAutoInsertTimeoutGet(sessionId, &hsAutoInsertTimeout);
            if (error != SA_OK) {
                status.assertError(TRACE, AUTO_INSERT_TIMEOUT_GET, SA_OK, error);
            } else {
                error = saHpiAutoExtractTimeoutGet(sessionId, resourceId, &hsAutoExtractTimeout);
                if (error != SA_OK) {
                    status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_GET, SA_OK, error);
                } else {
                    error = saHpiHotSwapStateGet(sessionId, resourceId, &hsState);
                    if (error != SA_OK) {
                        status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
                    } else {
                        hsIsHpiResource = false;
                        error = saHpiResourceIdGet(sessionId, &hpiResourceId);
                        if (error == SA_OK) {
                            if (hpiResourceId == resourceId) {
                                hsIsHpiResource = true;
                            }
                        } else if ((error != SA_ERR_HPI_NOT_PRESENT) &&
                                   (error != SA_ERR_HPI_UNKNOWN)) {
                            status.assertError(TRACE, RESOURCE_ID_GET, SA_OK, error);
                        }
                    }
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Does the resource have the Hotswap Indicator?
 *****************************************************************************/

bool HotSwapTestCase::isIndicatorSupported(SaHpiRptEntryT *rptEntry) {
    return (rptEntry->HotSwapCapabilities & SAHPI_HS_CAPABILITY_INDICATOR_SUPPORTED);
}

/*****************************************************************************
 * Restore the timeout values that were saved with the above method.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::restoreTimeouts(SaHpiSessionIdT sessionId,
                                               SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaErrorT error;

    if (gatherInfo) {
        if (!hsAutoInsertReadOnly) {
               error = saHpiAutoInsertTimeoutSet(sessionId, hsAutoInsertTimeout);
               status.checkError(TRACE, AUTO_INSERT_TIMEOUT_SET, error);
        }

        if (!hsAutoExtractReadOnly) {
               error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, hsAutoExtractTimeout);
               status.checkError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Set the timeouts to AUTO_TIMEOUT which will give the test case time
 * to cancel a policy.  If either timeout cannot be changed, the status
 * is set to NOTSUPPORT.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::setTimeouts(SaHpiSessionIdT sessionId,
                                           SaHpiResourceIdT resourceId,
                                           SaHpiTimeoutT timeout) {
    HpiTestStatus status;

    if (hsAutoInsertReadOnly || hsAutoExtractReadOnly) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAutoInsertTimeoutSet(sessionId, timeout);
        if (error != SA_OK) {
            status.assertError(TRACE, AUTO_INSERT_TIMEOUT_SET, SA_OK, error);
        } else {
            error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, timeout);
            status.checkError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Extract the resource, i.e. place the resource into the INACTIVE state. 
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::extractResource(SaHpiSessionIdT sessionId,
                                               SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiHsStateT state;
    TimeMsec timeout;

    SaErrorT error = saHpiHotSwapStateGet(sessionId, resourceId, &state);
    if (error != SA_OK) {
        status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
    } else if (state == SAHPI_HS_STATE_INACTIVE) {
        // do nothing
    } else if (state == SAHPI_HS_STATE_EXTRACTION_PENDING) {
        error = saHpiResourceInactiveSet(sessionId, resourceId);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_INACTIVE_SET, SA_OK, error);
        } else {
            status.add(TRACE, getAutoExtractionTime(sessionId, resourceId, &timeout));
            if (status.isOkay()) {
                status.add(TRACE, waitForHsEvent(sessionId, resourceId,
                                                 SAHPI_HS_STATE_INACTIVE, timeout));
            }
        }
    } else if (state == SAHPI_HS_STATE_ACTIVE) {
        error = saHpiHotSwapActionRequest(sessionId, resourceId,
                                          SAHPI_HS_ACTION_EXTRACTION);
        if (error != SA_OK) {
            status.assertError(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
        } else {
            status.add(TRACE, getAutoExtractionTime(sessionId, resourceId, &timeout));
            if (status.isOkay()) {
                status.add(TRACE, waitForHsEvent(sessionId, resourceId,
                                                 SAHPI_HS_STATE_INACTIVE, timeout));
            }
        }
    } else {
        status.assertError(TRACE, "Resource is not in a valid state for an extraction.");
    }

    return status;
}

/*****************************************************************************
 * Insert the resource, i.e. place the resource into the ACTIVE state. 
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::insertResource(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiHsStateT state;
    TimeMsec timeout;

    SaErrorT error = saHpiHotSwapStateGet(sessionId, resourceId, &state);
    if (error != SA_OK) {
        status.assertError(TRACE, HOT_SWAP_STATE_GET, SA_OK, error);
    } else if (state == SAHPI_HS_STATE_ACTIVE) {
        // do nothing
    } else if (state == SAHPI_HS_STATE_INSERTION_PENDING) {
        error = saHpiResourceActiveSet(sessionId, resourceId);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_ACTIVE_SET, SA_OK, error);
        } else {
            status.add(TRACE, getAutoInsertionTime(sessionId, &timeout));
            if (status.isOkay()) {
                status.add(TRACE, waitForHsEvent(sessionId, resourceId,
                                                 SAHPI_HS_STATE_ACTIVE, timeout));
            }
        }
    } else if (state == SAHPI_HS_STATE_INACTIVE) {

        error = saHpiHotSwapActionRequest(sessionId, resourceId,
                                          SAHPI_HS_ACTION_INSERTION);
        if (error != SA_OK) {
            status.assertError(TRACE, HOT_SWAP_ACTION_REQUEST, SA_OK, error);
        } else {
            status.add(TRACE, getAutoInsertionTime(sessionId, &timeout));
            if (status.isOkay()) {
                status.add(TRACE, waitForHsEvent(sessionId, resourceId,
                                                 SAHPI_HS_STATE_ACTIVE, timeout));
            }
        }
    } else {
        status.assertError(TRACE, "Resource is not in a valid state for an insertion.");
    }

    return status;
}

/*****************************************************************************
 * Get the entire time (msec) to wait for an auto insertion.
 * Add 1 second (1000 msec) to avoid any boundary situations.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::getAutoInsertionTime(SaHpiSessionIdT sessionId,
                                                    TimeMsec *time) {
    HpiTestStatus status;
    SaHpiTimeT timeout;

    SaErrorT error = saHpiAutoInsertTimeoutGet(sessionId, &timeout);
    if (error != SA_OK) {
        status.assertError(TRACE, AUTO_INSERT_TIMEOUT_GET, SA_OK, error);
    } else {
        *time = (timeout / 1000000) + getAutoInsertionPolicyTime() + 1000;
    }

    return status;
}

/*****************************************************************************
 * Get the entire time (msec) to wait for an auto extraction.
 * Add 1 second (1000 msec) to avoid any boundary situations.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::getAutoExtractionTime(SaHpiSessionIdT sessionId,
                                                     SaHpiResourceIdT resourceId,
                                                     TimeMsec *time) {
    HpiTestStatus status;
    SaHpiTimeT timeout;

    SaErrorT error = saHpiAutoExtractTimeoutGet(sessionId, resourceId, &timeout);
    if (error != SA_OK) {
        status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_GET, SA_OK, error);
    } else {
        *time = (timeout / 1000000) + getAutoExtractionPolicyTime() + 1000;
    }

    return status;
}

/*****************************************************************************
 * Wait for the given Hot Swap state event.
 *****************************************************************************/

HpiTestStatus HotSwapTestCase::waitForHsEvent(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId,
                                              SaHpiHsStateT state,
                                              TimeMsec timeout) {
    HpiTestStatus status;
    bool foundEvent;
    SaHpiEventT event;

    EventMatcher matcher;
    matcher.setSource(resourceId);
    matcher.setEventType(SAHPI_ET_HOTSWAP);
    matcher.setHotSwapState(state);

    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, timeout,
                                                &event, NULL, NULL, NULL, &foundEvent));
    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertError(TRACE, "Did not receive %s hot swap event.",
                               HpiString::hotSwapState(state));
        }
    }

    return status;
}

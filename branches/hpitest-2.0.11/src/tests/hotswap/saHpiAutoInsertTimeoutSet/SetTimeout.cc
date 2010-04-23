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

#include "SetTimeout.h"

using namespace ns_saHpiAutoInsertTimeoutSet;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define ONE_SECOND 1000000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetTimeout::SetTimeout(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetTimeout::getName() {
    return "SetTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetTimeout::getDescription() {
    return "Set the timeout for how long to wait before the default\n"
           "auto-insertion policy is invoked on a resource.  Try several\n"
           "legal timeout values: one second, SAHPI_TIMEOUT_IMMEDIATE,\n"
           "and SAHPI_TIMEOUT_BLOCK.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetTimeout::getPrecondition() {
    return "Requires a domain whose Auto Insert Timeout is not read only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetTimeout::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetTimeout::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;
    SaHpiTimeoutT origTimeout;
    SaHpiTimeoutT newTimeout;
    SaHpiTimeoutT timeout[] = { ONE_SECOND,
                                SAHPI_TIMEOUT_IMMEDIATE,
                                SAHPI_TIMEOUT_BLOCK };

    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else if (domainInfo.DomainCapabilities & SAHPI_DOMAIN_CAP_AUTOINSERT_READ_ONLY) {
        status.assertNotSupport();
    } else {
        error = saHpiAutoInsertTimeoutGet(sessionId, &origTimeout);
        if (error != SA_OK) {
            status.assertError(TRACE, AUTO_INSERT_TIMEOUT_GET, SA_OK, error);
        } else {
            for (int i = 0; i < 3; i++) {
                error = saHpiAutoInsertTimeoutSet(sessionId, timeout[i]);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, AUTO_INSERT_TIMEOUT_SET, SA_OK, error);
                } else {
                    error = saHpiAutoInsertTimeoutGet(sessionId, &newTimeout);
                    if (error != SA_OK) {
                        status.assertError(TRACE, AUTO_INSERT_TIMEOUT_GET, SA_OK, error);
                    } else if (newTimeout == timeout[i]) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, "Set failed. "
                                "Retrieved timeout of %s did not match set timeout of %s.",
                                HpiString::timeout(newTimeout), 
                                HpiString::timeout(timeout[i]));
                    }
                }
            }

            error = saHpiAutoInsertTimeoutSet(sessionId, origTimeout);
            status.checkError(TRACE, AUTO_INSERT_TIMEOUT_SET, error);
        }
    }

    return status;
}

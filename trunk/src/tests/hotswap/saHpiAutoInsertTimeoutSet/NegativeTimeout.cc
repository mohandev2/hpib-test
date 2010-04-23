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

#include "NegativeTimeout.h"

using namespace ns_saHpiAutoInsertTimeoutSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NegativeTimeout::NegativeTimeout(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NegativeTimeout::getName() {
    return "NegativeTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NegativeTimeout::getDescription() {
    return "The <i>Timeout</i> parameter is a passed in with a negative\n"
           "value, i.e. it is not SAHPI_TIMEOUT_BLOCK, SAHPI_TIMEOUT_IMMEDIATE,\n"
           "or a postive value.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NegativeTimeout::getPrecondition() {
    return "Requires a domain whose Auto Insert Timeout is not read only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NegativeTimeout::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NegativeTimeout::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;

    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else if (domainInfo.DomainCapabilities & SAHPI_DOMAIN_CAP_AUTOINSERT_READ_ONLY) {
        status.assertNotSupport();
    } else {
        error = saHpiAutoInsertTimeoutSet(sessionId, -2);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, AUTO_INSERT_TIMEOUT_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

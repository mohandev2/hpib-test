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

#include "ReadOnly.h"

using namespace ns_saHpiAutoInsertTimeoutSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnly::ReadOnly(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnly::getName() {
    return "ReadOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnly::getDescription() {
    return "The auto-insert timeout for a domain is fixed as indicated\n"
           "byt the SAHPI_DOMAIN_CAP_AUTOINSERT_READ_ONLY flag in the\n"
           "<i>DomainInfo</i> structure.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires a domain whos Auto Insert Timeout is read only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnly::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReadOnly::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;

    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else if (!(domainInfo.DomainCapabilities & SAHPI_DOMAIN_CAP_AUTOINSERT_READ_ONLY)) {
        status.assertNotSupport();
    } else {
        error = saHpiAutoInsertTimeoutSet(sessionId, SAHPI_TIMEOUT_IMMEDIATE);
        if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, AUTO_INSERT_TIMEOUT_SET,
                                 SA_ERR_HPI_READ_ONLY, error);
        }
    }

    return status;
}

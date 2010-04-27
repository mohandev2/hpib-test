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

#include "ValidDomainId.h"

using namespace ns_saHpiSessionOpen;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ValidDomainId::ValidDomainId(char *line) : HpiGenTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ValidDomainId::getName() {
    return "ValidDomainId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ValidDomainId::getDescription() {
    return "Open a session using a specific valid Domain Id.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ValidDomainId::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Open a session with SAHPI_UNSPECIFIED_DOMAIN_ID in order to retrieve a
 * valid domain id.  Then use that explicit domain id to open a session.
 *****************************************************************************/

HpiTestStatus ValidDomainId::run() {
    HpiTestStatus status;
    SaHpiSessionIdT sessionId1;
    SaHpiSessionIdT sessionId2;
    SaHpiDomainInfoT domainInfo;

    SaErrorT error = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID, 
                                      &sessionId1, NULL);
    if (error != SA_OK) {
        status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
    } else {
        error = saHpiDomainInfoGet(sessionId1, &domainInfo);
        if (error != SA_OK) {
            status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
        } else {
            error = saHpiSessionOpen(domainInfo.DomainId, &sessionId2, NULL);
            if (error != SA_OK) {
                status.assertFailure(TRACE, SESSION_OPEN, SA_OK, error);
            } else {
                status.assertPass();
                error = saHpiSessionClose(sessionId2);
                status.checkError(TRACE, SESSION_CLOSE, error);
            }

            error = saHpiSessionClose(sessionId1);
            status.checkError(TRACE, SESSION_CLOSE, error);
        }
    }

    return status;
}


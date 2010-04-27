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

#include "TwoSessions.h"
#include "TextBufferHelper.h"
#include "Report.h"

using namespace ns_saHpiDomainTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TwoSessions::TwoSessions(char *line) : HpiGenTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TwoSessions::getName() {
    return "TwoSessions";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TwoSessions::getDescription() {
    return "Verify that a change to the domain's tag immediately "
           "becomes visible in another session on the same domain.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT TwoSessions::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus TwoSessions::run() {
    HpiTestStatus status;
    Report report;
    SaHpiSessionIdT sessionId1;
    SaHpiSessionIdT sessionId2;
    SaHpiTextBufferT domainTag;
    SaHpiDomainInfoT domainInfo1;
    SaHpiDomainInfoT domainInfo2;

    SaErrorT error = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID,
                                      &sessionId1, NULL);
    if (error != SA_OK) {
        status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
    } else {
        error = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID,
                                 &sessionId2, NULL);
        if (error != SA_OK) {
            status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
        } else {

            // get the original domain tag so we restore it later

            error = saHpiDomainInfoGet(sessionId1, &domainInfo1);
            if (error != SA_OK) {
                status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
            } else {

                TextBufferHelper::fill(&domainTag);

                error = saHpiDomainTagSet(sessionId1, &domainTag);
                if (error != SA_OK) {
                    status.assertError(TRACE, DOMAIN_TAG_SET, SA_OK, error);
                } else {
                    error = saHpiDomainInfoGet(sessionId2, &domainInfo2);
                    if (error != SA_OK) {
                        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
                    } else if (!TextBufferHelper::isEqual(&domainTag,
                                                          &domainInfo2.DomainTag,
                                                          report)) {
                        status.assertFailure(TRACE, report.toString());
                    } else {
                        status.assertPass();
                    }
                }

                // restore the original domain tag

                error = saHpiDomainTagSet(sessionId1, &domainInfo1.DomainTag);
                status.checkError(TRACE, DOMAIN_TAG_SET, error);
            }
        }
    }

    return status;
}

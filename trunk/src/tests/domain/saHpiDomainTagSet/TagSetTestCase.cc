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

#include "TagSetTestCase.h"

using namespace ns_saHpiDomainTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TagSetTestCase::TagSetTestCase(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Wrap the saving and restoration of the Domain Tag around the actual test.
 *****************************************************************************/

HpiTestStatus TagSetTestCase::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;

    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else {
        status = runTagTest(sessionId);

        error = saHpiDomainTagSet(sessionId, &domainInfo.DomainTag);
        status.checkError(TRACE, DOMAIN_TAG_SET, error);
    }

    return status;
}

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

#include "SetDomainTag.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiDomainTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetDomainTag::SetDomainTag(char *line) : TagSetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetDomainTag::getName() {
    return "SetDomainTag";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetDomainTag::getDescription() {
    return "Set the domain's tag and verify that the change was successful.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetDomainTag::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetDomainTag::runTagTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;
    SaHpiTextBufferT domainTag;

    TextBufferHelper::fill(&domainTag);

    SaErrorT error = saHpiDomainTagSet(sessionId, &domainTag);
    if (error != SA_OK) {
        status.assertFailure(TRACE, DOMAIN_TAG_SET, SA_OK, error);
    } else {
        error = saHpiDomainInfoGet(sessionId, &domainInfo);
        if (error != SA_OK) {
            status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
        } else if (!TextBufferHelper::isEqual(&domainTag, &domainInfo.DomainTag)) {
            status.assertFailure(TRACE, "Failed to change the Domain Tag.");
        } else {
            status.assertPass();
        }
    }

    return status;
}

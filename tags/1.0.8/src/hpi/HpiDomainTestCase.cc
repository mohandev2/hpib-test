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

#include "HpiTestStatus.h"
#include "HpiDomainTestCase.h"
#include "LocationMgr.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiDomainTestCase::HpiDomainTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiDomainTestCase::HpiDomainTestCase(char *line) : HpiTestCase(line) { 
}

/*****************************************************************************
 * Test each domain in the HPI system.
 *****************************************************************************/

TestStatus HpiDomainTestCase::runTest() {
    HpiTestStatus status;

    IdList &domainIdList = getDomainIdList();
    for (int i = 0; i < domainIdList.getCount() && !status.hasFault(); i++) {
        SaHpiDomainIdT domainId = domainIdList.getId(i);
        status.merge(testDomain(domainId));
    }

    return status;
}

/*****************************************************************************
 * Test a single domain.
 *****************************************************************************/

HpiTestStatus HpiDomainTestCase::testDomain(SaHpiDomainIdT domainId) {

    LocationMgr::push("Domain", domainId);
    HpiTestStatus status = runDomainTest(domainId);
    LocationMgr::pop();

    return status;
}

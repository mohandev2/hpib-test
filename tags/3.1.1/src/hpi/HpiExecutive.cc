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
#include <stdlib.h>

#include "HpiExecutive.h"
#include "HpiProperties.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

HpiExecutive::HpiExecutive(BaseTestSuite *ts)
: Executive(ts) {
    properties = new HpiProperties();
    properties->initialize(".hpitest");
}

/*****************************************************************************
 * Destructor
 *****************************************************************************/

HpiExecutive::~HpiExecutive() {
}

/*****************************************************************************
 * Initialization before executing the test cases.
 *****************************************************************************/

void HpiExecutive::initialize() {
    domainIdList.clear();
    HpiTestStatus status = buildDomainIdList(SAHPI_UNSPECIFIED_DOMAIN_ID);

    if (status.hasFault()) {
        std::cout << status.toString().c_str() << std::endl;
        exit(1);
    }
}

/*****************************************************************************
 * Return the HPI properties.
 *****************************************************************************/

HpiProperties *HpiExecutive::getProperties() {
    return properties;
}

/*****************************************************************************
 * Return the domain IDs.
 *****************************************************************************/

IdList &HpiExecutive::getDomainIdList() {
    return domainIdList;
}

/*****************************************************************************
 * Recursively find all of the domain IDs in the HPI system.
 *****************************************************************************/

HpiTestStatus HpiExecutive::buildDomainIdList(SaHpiDomainIdT domainId) {
    HpiTestStatus status;
    SaHpiSessionIdT sessionId;
    SaHpiDomainInfoT domainInfo;

    SaErrorT error = saHpiSessionOpen(domainId, &sessionId, NULL);
    if (error != SA_OK) {
        status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
    } else {
        if (properties->isForceDiscovery()) {
             saHpiDiscover(sessionId);
        }

        error = saHpiDomainInfoGet(sessionId, &domainInfo);
        if (error != SA_OK) {
            status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
        } else {
            domainIdList.add(domainInfo.DomainId);

            SaHpiDrtEntryT drtEntry;
            SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
            while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY) {
                SaHpiEntryIdT entryId = nextEntryId;
                error = saHpiDrtEntryGet(sessionId, entryId, &nextEntryId, &drtEntry);
                if (error == SA_ERR_HPI_NOT_PRESENT) {             
                    break;         
                } else if (error != SA_OK) {
                    status.assertError(TRACE, DRT_ENTRY_GET, SA_OK, error);
                } else if (!domainIdList.contains(drtEntry.DomainId)) {
                    status = buildDomainIdList(drtEntry.DomainId);
                }
            }
        }
    }

    return status;
} 

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

#include "Scope.h"
#include "ResourceHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiResourceTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Scope::Scope(char *line) : TagSetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Scope::getName() {
    return "Scope";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Scope::getDescription() {
    return "Since the resource tag is contained within an RPT entry,\n"
           "its scope is limited to a single domain.  A resource that\n"
           "exist in more than one domain will have independent resource\n"
           "tags.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Scope::getPrecondition() {
    return "Requires a peer domain.";
}

/*****************************************************************************
 * Run the test.
 *
 * If we have a peer domain, change the resource tag in this domain and
 * verify that the same resource in the peer domain did not have its tag
 * changed.
 *****************************************************************************/

HpiTestStatus Scope::runTagTest(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiTextBufferT tag;
    SaHpiDomainIdT peerDomainId;
    SaHpiSessionIdT peerSessionId;
    SaHpiRptEntryT peerRptEntry;
    SaHpiRptEntryT testRptEntry;
    bool foundPeerDomain;
    bool foundResource;

    status.add(TRACE, getPeerDomain(sessionId, &peerDomainId, &foundPeerDomain));
    if (status.isOkay()) {
        if (!foundPeerDomain) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiSessionOpen(peerDomainId, &peerSessionId, NULL);
            if (error != SA_OK) {
                status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
            } else {
                status.add(TRACE, findResource(peerSessionId, rptEntry->ResourceId,
                                               &rptEntry->ResourceEntity,
                                               &peerRptEntry, &foundResource));
                if (status.isOkay()) {
                    if (!foundResource) {
                        status.assertError(TRACE, "Unable to find resource 0x%x (%s) in peer domain.", 
                                           rptEntry->ResourceId,
                                           HpiString::entityPath(&rptEntry->ResourceEntity));
                    } else {
                        TextBufferHelper::fill(&tag, "Mickey Mouse");
                        error = saHpiResourceTagSet(sessionId, rptEntry->ResourceId, &tag);
                        if (error != SA_OK)     {
                            status.assertError(TRACE, RESOURCE_TAG_SET, SA_OK, error);
                        } else {
                            error = saHpiRptEntryGetByResourceId(peerSessionId, 
                                                                 peerRptEntry.ResourceId,
                                                                 &testRptEntry);
                            if (error != SA_OK) {
                                status.assertError(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
                            } else if (TextBufferHelper::isEqual(&tag, &testRptEntry.ResourceTag)) {
                                status.assertFailure(TRACE, "The resource tag in the peer domain "
                                                            "matches the changed resource tag.");
                            } else {
                                status.assertPass();
                            }
                        }
                    }
                }

                error = saHpiSessionClose(peerSessionId);
                status.checkError(TRACE, SESSION_CLOSE, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Find the peer domain for the given domain, if there is one.
 *****************************************************************************/

HpiTestStatus Scope::getPeerDomain(SaHpiSessionIdT sessionId,
                                   SaHpiDomainIdT *domainId, bool *found) {
    HpiTestStatus status;
    SaHpiDomainInfoT domainInfo;
    SaHpiEntryIdT nextEntryId;
    SaHpiDrtEntryT domainTableEntry;

    *found = false;
    SaErrorT error = saHpiDomainInfoGet(sessionId, &domainInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else if (domainInfo.IsPeer) {
        error = saHpiDrtEntryGet(sessionId, SAHPI_FIRST_ENTRY, &nextEntryId, &domainTableEntry);
        if (error != SA_OK) {
            status.assertError(TRACE, DRT_ENTRY_GET, SA_OK, error);
        } else {
            *domainId = domainTableEntry.DomainId;
            *found = true;
        }
    }
    
    return status;
}

/*****************************************************************************
 * Find the resource associated with the given Entity Path.
 *****************************************************************************/

HpiTestStatus Scope::findResource(SaHpiSessionIdT sessionId, 
                                  SaHpiResourceIdT resourceId, 
                                  SaHpiEntityPathT *entityPath, 
                                  SaHpiRptEntryT *rptEntry, 
                                  bool *found) {

    HpiTestStatus status;

    *found = false;
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId, &nextEntryId, rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else if ((resourceId == rptEntry->ResourceId) &&
                   (ResourceHelper::isEqual(entityPath, &rptEntry->ResourceEntity))) {
            *found = true;
            break;
        }
    }

    return status;
}

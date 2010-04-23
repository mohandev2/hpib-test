/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 *
 * Changes:
 * 2009/11/02 larswetzel@users.sourceforge.net
 *            Don't break the fill process if a resource is failed and 
 *            RdrUpdateCount couldn't be read for this reason 
 */

#include "ResourcesList.h"
#include "ResourcesListEntry.h"
#include "HpiString.h"


/*****************************************************************************
 * Constructor.
 *****************************************************************************/

ResourcesList::ResourcesList() {
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

ResourcesList::ResourcesList(ResourcesList &resourcesList) {
    ListIterator iter = resourcesList.rptlist.iterator();
    while (iter.hasNext()) {
        ResourcesListEntry *rle = (ResourcesListEntry *) iter.next();
        add(new ResourcesListEntry(*rle));
    }
}

/*****************************************************************************
 * Fill the resources list 
 *****************************************************************************/
HpiTestStatus ResourcesList::fill(SaHpiSessionIdT sessionId) {
	HpiTestStatus status;
    
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaHpiRptEntryT *rptEntry = new SaHpiRptEntryT;
        	
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId,
                                          &nextEntryId, rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else {
            SaHpiUint32T updCnt;
            
            error = saHpiRdrUpdateCountGet(sessionId, rptEntry->ResourceId, &updCnt);
            if (rptEntry->ResourceFailed && (error == SA_ERR_HPI_NO_RESPONSE)) {
            	    ResourcesListEntry *rle = new ResourcesListEntry(rptEntry->ResourceId, 0, rptEntry);
            	    rptlist.add(rle);
            } else if (error != SA_OK) {
        	        status.assertError(TRACE, RDR_UPDATE_COUNT_GET, SA_OK, error);
                break;
            } else {
                ResourcesListEntry *rle = new ResourcesListEntry(rptEntry->ResourceId, updCnt, rptEntry);
                rptlist.add(rle);
            }        		
        }
    }

    return status;

}

/*****************************************************************************
 * Destructor.  
 *
 * Delete all of the Resources List entries.
 *****************************************************************************/

ResourcesList::~ResourcesList() {
    ListIterator iter = rptlist.iterator();
    while (iter.hasNext()) {
        ResourcesListEntry *rle = (ResourcesListEntry *) iter.next();
        delete rle;
    }
}

/*****************************************************************************
 * Add a resource list entry.
 *****************************************************************************/

void ResourcesList::add(ResourcesListEntry *entry) {
    rptlist.add(entry);
}

/*****************************************************************************
 * Find the resource list entry with the same resourceId.
 * If no resource list entry can found, SA_ERR_HPI_INVALID_RESOURCE is returned.
 *****************************************************************************/

SaErrorT ResourcesList::getUpdateCount(SaHpiResourceIdT resourceId, SaHpiUint32T *updateCount) {
    ListIterator iter = rptlist.iterator();
    SaHpiResourceIdT rid;

    while (iter.hasNext()) {
            ResourcesListEntry *rle;
        rle = (ResourcesListEntry *) iter.next();
        rid = rle->getResourceId();

        if (rid == resourceId) {
                *updateCount = rle->getUpdateCount();
                return SA_OK;
        }
    }
    return SA_ERR_HPI_INVALID_RESOURCE;
}

SaErrorT ResourcesList::getRptEntry(SaHpiResourceIdT resourceId, SaHpiRptEntryT *rptData) {
    ListIterator iter = rptlist.iterator();
    SaHpiResourceIdT rid;

    while (iter.hasNext()) {
            ResourcesListEntry *rle;
        rle = (ResourcesListEntry *) iter.next();
        rid = rle->getResourceId();
        if (rid == resourceId) {
            *rptData = *(rle->getRptEntry());
                return SA_OK;
        }
    }
    return SA_ERR_HPI_INVALID_RESOURCE;
}

SaErrorT ResourcesList::getEntityPath(SaHpiResourceIdT resourceId, SaHpiEntityPathT *ep) {
    SaHpiRptEntryT tmp;
    
    SaErrorT ret = getRptEntry(resourceId, &tmp);
    *ep = *(&tmp.ResourceEntity);
    return ret;
}

SaErrorT ResourcesList::isHotSwapFRU(SaHpiResourceIdT resourceId, bool *isHSFRU) {
    SaHpiRptEntryT tmp;
    
    *isHSFRU = false;
    SaErrorT ret = getRptEntry(resourceId, &tmp);
    if (tmp.ResourceCapabilities & 
        (SAHPI_CAPABILITY_FRU | SAHPI_CAPABILITY_MANAGED_HOTSWAP))  
        *isHSFRU = true;
            
    return ret;    
}

/*****************************************************************************
 * Are there any resource list entries?
 *****************************************************************************/

bool ResourcesList::isEmpty() {
    return rptlist.isEmpty();
}

/*****************************************************************************
 * Return an iterator to traverse the resource list entries.
 *****************************************************************************/

ListIterator ResourcesList::iterator() const {
    return rptlist.iterator();
}

/*****************************************************************************
 * For Debug purpose or an potential verbose mode
 *****************************************************************************/
void ResourcesList::printData() {
    ListIterator iter = rptlist.iterator();

    while (iter.hasNext()) {
        ResourcesListEntry *rle;
        rle = (ResourcesListEntry *) iter.next();
        printf("\nElement:\n");
        rle->printData();
    }
}


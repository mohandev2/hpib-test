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
 */

#include <cstdio>
#include "ResourcesListEntry.h"
#include "HpiString.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

ResourcesListEntry::ResourcesListEntry(SaHpiResourceIdT resourceId, 
                                       SaHpiUint32T updateCount,
                                       SaHpiRptEntryT *rptdata) {
    this->resId = resourceId;
    this->updCnt = updateCount;
    this->rptEntry = rptdata;
}

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

ResourcesListEntry::ResourcesListEntry(SaHpiResourceIdT resourceId, 
                                       SaHpiUint32T updateCount) {
    ResourcesListEntry(resourceId, updateCount, NULL);
}

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

ResourcesListEntry::ResourcesListEntry(SaHpiRptEntryT *rptdata) {
    ResourcesListEntry(rptdata->ResourceId, 0, rptdata);
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

ResourcesListEntry::ResourcesListEntry(ResourcesListEntry &rle) {
    this->resId  = rle.resId;
    this->updCnt = rle.updCnt;
    this->rptEntry = rle.rptEntry;
}

/*****************************************************************************
 * Return the ResourceId.
 *****************************************************************************/

SaHpiResourceIdT ResourcesListEntry::getResourceId() {
    return resId;
}

/*****************************************************************************
 * Return the Rdr Update Count.
 *****************************************************************************/

SaHpiUint32T ResourcesListEntry::getUpdateCount() {
    return updCnt;
}

/*****************************************************************************
 * Return the rpt entry pointer
 ******************************************************************************/
SaHpiRptEntryT* ResourcesListEntry::getRptEntry() {
	return rptEntry;
}

/*****************************************************************************
 * For debug purposes or an potential verbose mode
 *****************************************************************************/
void ResourcesListEntry::printData() {
	printf("\tUpdateCount: %d\n", updCnt);
	printf("\tResourceId:  %d\n", resId);
	printf("\tEntityPath:  %s\n", HpiString::entityPath(&rptEntry->ResourceEntity));
	if (rptEntry->ResourceCapabilities 
		& (SAHPI_CAPABILITY_FRU | SAHPI_CAPABILITY_MANAGED_HOTSWAP)) 
		printf("\tCapabilityField includes FRU and HS functionality\n");
}

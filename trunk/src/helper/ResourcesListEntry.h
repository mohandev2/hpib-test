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

#ifndef __RESOURCESLISTENTRY_H__
#define __RESOURCESLISTENTRY_H__

#include "SafTest.h"

/*****************************************************************************
 * Resources List Entry
 *
 * Each entry consists of the resourceId and the rdr update count.
 *****************************************************************************/

class ResourcesListEntry
{
private:
    SaHpiResourceIdT resId;
    SaHpiUint32T     updCnt;
	SaHpiRptEntryT   *rptEntry;
	
public:
	ResourcesListEntry(SaHpiRptEntryT *rptdata);
    ResourcesListEntry(SaHpiResourceIdT resourceId, SaHpiUint32T updateCount);
    ResourcesListEntry(SaHpiResourceIdT resourceId, SaHpiUint32T updateCount, SaHpiRptEntryT *rptdata);
    ResourcesListEntry(ResourcesListEntry &rle);

    SaHpiResourceIdT getResourceId();
    SaHpiUint32T     getUpdateCount();
    SaHpiRptEntryT*  getRptEntry();
    void printData();

};

#endif

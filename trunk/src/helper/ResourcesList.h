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

#ifndef __RESOURCESLIST_H__
#define __RESOURCESLIST_H__

#include <string>
#include "List.h"
#include "ListIterator.h"
#include "ResourcesListEntry.h"
#include "HpiTestStatus.h"


/*****************************************************************************
 * Resources List
 *
 * Used to determine if something was changed during the test. 
 * The resources list contains all ResourceIds, Rdr update counts and the Rpt entries. 
 * This list can be enhanced by further values if needed.
 *****************************************************************************/

class ResourcesList
{
private:
    List rptlist;

public:
    ResourcesList();
    ResourcesList(ResourcesList &ResourcesList);
    ~ResourcesList();

    HpiTestStatus	fill(SaHpiSessionIdT sessionId);
	void			add(ResourcesListEntry *entry);
    SaErrorT			getUpdateCount(SaHpiResourceIdT resourceId, SaHpiUint32T *updateCount);
    SaErrorT			getRptEntry(SaHpiResourceIdT resourceId, SaHpiRptEntryT *rptData);    
    SaErrorT			getEntityPath(SaHpiResourceIdT resourceId, SaHpiEntityPathT *ep);
    SaErrorT			isHotSwapFRU(SaHpiResourceIdT resourceId, bool *isHotSwapFRU);      
    bool 			isEmpty();
    void			printData();
    ListIterator 	iterator() const;
};

#endif

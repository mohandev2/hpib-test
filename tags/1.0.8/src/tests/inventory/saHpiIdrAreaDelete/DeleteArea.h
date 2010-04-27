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

#ifndef __DELETEAREA_H__
#define __DELETEAREA_H__

#include "SafTest.h"
#include "AddIdrTestCase.h"

/*****************************************************************************
 * Delete Area Test Case
 *****************************************************************************/

namespace ns_saHpiIdrAreaDelete
{
    class DeleteArea : public AddIdrTestCase
    {
    public:
        DeleteArea(char *line);

        HpiTestStatus runAddIdrTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry, 
                                    SaHpiInventoryRecT *idrRec,
                                    SaHpiEntryIdT areaId);

        const char *getName();

        const char *getDescription();

        const char *getPrecondition();

        SaErrorT getExpectedReturn();
    };
}

#endif

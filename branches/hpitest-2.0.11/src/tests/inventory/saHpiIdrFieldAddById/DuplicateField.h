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
 *      Akmal A. Muqeeth <aax6@unh.edu>
 *
 * 09/07/30 lars.wetzel@emerson.com
 *          change class of parent (to run the test also in a ReadOnly-Idr)
 */

#ifndef __DUPLICATEFIELD_H__
#define __DUPLICATEFIELD_H__

#include "SafTest.h"
#include "WriteIdrTestCase.h"

/*****************************************************************************
 * Delete Area Test Case
 *****************************************************************************/

namespace ns_saHpiIdrFieldAddById
{
    class DuplicateField : public WriteIdrTestCase
    {
    public:
        DuplicateField(char *line);

        HpiTestStatus runWriteIdrTest(SaHpiSessionIdT sessionId,
				      SaHpiRptEntryT *rptEntry, 
				      SaHpiInventoryRecT *idrRec);

        const char *getName();

        const char *getDescription();

        const char *getPrecondition();

        SaErrorT getExpectedReturn();
    };
}

#endif

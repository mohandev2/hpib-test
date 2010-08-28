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
 *     Meghana  Srivatsavai <mjn77@unh.edu>
 * 
 * Changes:
 * 09/12/28 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Integrate the test case into hpitest
 */

#ifndef __GETDISABLE_H__
#define __GETDISABLE_H__

#include "SafTest.h"
#include "FumiTestCase.h"

/*****************************************************************************
 * Okay Test Case
 *****************************************************************************/

namespace ns_saHpiFumiAutoRollbackDisableGet
{
    class GetDisable : public FumiTestCase
    {
    public:
        GetDisable(char *line);

        HpiTestStatus runFumiTest(SaHpiSessionIdT sessionId,
                                  SaHpiRptEntryT *rptEntry,
                                  SaHpiFumiRecT *fumiRec);

        const char *getName();

        const char *getDescription();

        SaErrorT getExpectedReturn();
    };
}

#endif

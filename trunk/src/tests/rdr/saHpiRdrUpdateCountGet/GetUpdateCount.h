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

#ifndef __GETUPDATECOUNT_H__
#define __GETUPDATECOUNT_H__

#include "SafTest.h"
#include "HpiSessionTestCase.h"

/*****************************************************************************
 * Get Rdr Update Count Test Case
 *****************************************************************************/

namespace ns_saHpiRdrUpdateCountGet
{
    class GetUpdateCount : public HpiSessionTestCase
    {
    public:
        GetUpdateCount(char *line);

        HpiTestStatus runSessionTest(SaHpiSessionIdT sessionId);

        const char *getName();

        const char *getDescription();

        SaErrorT getExpectedReturn();

        bool isManual();

    private:
        HpiTestStatus updateFRU(SaHpiSessionIdT sessionId, SaHpiResourceIdT *updatedResourceId);
    };
}

#endif

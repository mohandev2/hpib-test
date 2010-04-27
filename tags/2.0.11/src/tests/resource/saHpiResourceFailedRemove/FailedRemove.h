/*      
 * (C) Copyright Emerson Corp. 2009
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Lars Wetzel <Lars.Wetzel.com>
 */

#ifndef __FAILEDREMOVE_H__
#define __FAILEDREMOVE_H__

#include "SafTest.h"
#include "HpiResourceTestCase.h"

/*****************************************************************************
 * No FRU Capability Test Case
 *****************************************************************************/

namespace ns_saHpiResourceFailedRemove
{
    class FailedRemove : public HpiResourceTestCase
    {
    public:
        FailedRemove(char *line);

        HpiTestStatus runResourceTest(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry);

        const char *getName();

        const char *getDescription();

        const char *getPrecondition();

        SaErrorT getExpectedReturn();
    };
}

#endif

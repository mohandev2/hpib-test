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

#ifndef __POLICYCANCEL_H__
#define __POLICYCANCEL_H__

#include "HotSwapTestCase.h"

/*****************************************************************************
 * Policy Cancel Test Case
 *****************************************************************************/

namespace ns_saHpiHotSwapPolicyCancel
{
    class PolicyCancel : public HotSwapTestCase
    {
    public:
        PolicyCancel(char *line);

        HpiTestStatus runHsTest(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry);

        const char *getName();

        const char *getDescription();

        const char *getPrecondition();

        SaErrorT getExpectedReturn();

    private:
        HpiTestStatus testResource(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId);

        HpiTestStatus extractTest(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId,
                                  SaHpiTimeoutT timeout);

        HpiTestStatus insertTest(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId,
                                 SaHpiTimeoutT timeout);
    };
}

#endif

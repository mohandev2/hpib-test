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
 *     Meghana Srivatsavai <mjn77@unh.edu>
 */

#ifndef __LOGICALTARGETINFOGET_TESTSUITE_H__
#define __LOGICALTARGETINFOGET_TESTSUITE_H__

#include "HpiTestSuite.h"

/*****************************************************************************
 * saHpiFumiLogicalTargetInfoGet Test Suite
 *****************************************************************************/

namespace ns_saHpiFumiLogicalTargetInfoGet
{
    class TestSuite : public HpiTestSuite
    {
    public:
        TestSuite();

        const char *getName();
    
        const char *getDescription();
    };
}

#endif
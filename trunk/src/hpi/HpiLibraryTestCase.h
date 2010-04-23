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
 
#ifndef __HPILIBRARYTESTCASE_H___
#define __HPILIBRARYTESTCASE_H___

#include "SafTest.h"
#include "HpiTestCase.h"
#include "HpiTestStatus.h"

/*****************************************************************************
 * HPI Library Test Case
 *
 * Run a test that requires a library in initial state. 
 *****************************************************************************/

class HpiLibraryTestCase : public HpiTestCase
{
public:
    HpiLibraryTestCase();
    HpiLibraryTestCase(char* line);

protected:
    TestStatus runTest();

    virtual HpiTestStatus runLibraryTest() = 0;
};

#endif

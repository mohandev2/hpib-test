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

#ifndef __EXECUTIVE_H__
#define __EXECUTIVE_H__

#include <cstdio>
#include <iostream>
#include <fstream>

#include "BaseTestSuite.h"
#include "BaseTestCase.h"
#include "UserPrompter.h"
#include "List.h"

using namespace std;

/*****************************************************************************
 * Executive
 *
 * Controls the execution of the test cases.
 *****************************************************************************/

class Executive
{
private:
    BaseTestSuite *testSuite;
    UserPrompter *prompter;

public:
    Executive(BaseTestSuite *testSuite);
    virtual ~Executive();

    virtual void initialize();

    BaseTestSuite *getTestSuite();

    void execute(List &includedTests,
                 List &excludedTests,
                 bool includeManual,
                 bool appendToLogFile);

    void prompt(const char *message);

private:
    bool isExcluded(BaseTestCase *tc, List &excludedList);
};

#endif

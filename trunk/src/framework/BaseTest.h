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

#ifndef __BASETEST_H__
#define __BASETEST_H__

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include "TestStatus.h"
#include "StringBuffer.h"
#include "Time.h"
#include "List.h"
#include "HtmlToc.h"

class Executive;

using namespace std;

/*****************************************************************************
 * Base class for all Tests.
 *****************************************************************************/

class BaseTest 
{
private:
    BaseTest *parent;
    char *path;

protected:
    BaseTest();
    virtual ~BaseTest();

public:

    virtual const char* getName() = 0;
    virtual const char* getDescription() = 0;

    const char *getPath();

    void setParent(BaseTest *test);

    virtual bool isTestCase() = 0;

    virtual void getTestCases(List &list) = 0;

    virtual BaseTest *find(const char *name) = 0;

    virtual bool isChildOf(BaseTest *test);

    virtual void printList();

    virtual int getTestCaseCount() = 0;
};

#endif

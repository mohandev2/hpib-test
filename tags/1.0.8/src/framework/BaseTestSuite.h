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

#ifndef __BASETESTSUITE_H__
#define __BASETESTSUITE_H__

#include <iostream>
#include <fstream>
#include <string>

#include "List.h"
#include "BaseTest.h"
#include "TestStatus.h"

using namespace std;

class Executive;

/*****************************************************************************
 * A Test Suite contains a set of tests.
 *****************************************************************************/

class BaseTestSuite : public BaseTest
{
private:
    List children;

public:
    BaseTestSuite();
    virtual ~BaseTestSuite();

    bool isTestCase();

    List &getChildren();

    void getTestCases(List &list);

    BaseTest *find(const char *name);

    void printList();

    HtmlToc *makeHtml(const char *dirname);

    int getTestCaseCount();

    virtual const char *getTitle() = 0;
    virtual const char *getBanner() = 0;

protected:
    void add(BaseTest *child);

private:
    void makeWebPage(const char *dirname);
};

#endif


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

#ifndef __BASETESTCASE_H__
#define __BASETESTCASE_H__

#include <iostream>
#include <fstream>
#include <cstring>

#include "BaseTest.h"
#include "UserPrompter.h"
#include "TestStatus.h"
#include "Util.h"
#include "List.h"

using namespace std;

class Executive;
class StringTokenizer;

/*****************************************************************************
 * A Test Case is responsible for testing a specific item.
 *****************************************************************************/

class BaseTestCase : public BaseTest
{
private:
    char *testCaseLine;
    int blockId;
    TestStatus testCaseStatus;
    Executive *executive;

public:
    void execute(Executive *executive);

    bool isTestCase();

    void saveReport(ostream &out);

    static void testBlockCallback(void *data);

    void getTestCases(List &list);
    BaseTest *find(const char *name);

    virtual bool isManual();

    TestStatus &getStatus();

    void addHtml(ofstream &out);

    int getTestCaseCount();

protected:
    BaseTestCase();
    BaseTestCase(char *line);
    virtual ~BaseTestCase();

    virtual TestStatus runTest() = 0;
    virtual const char *getPrecondition();

    virtual int getTimeout() = 0;

    void pushLocation(const char *name, unsigned int id);
    void popLocation();

    virtual void pauseTimer();
    virtual void resumeTimer();

    Executive *getExecutive();

    void secSleep(unsigned int seconds);
    void msecSleep(long msec);
    void usecSleep(long usec);

    virtual void fillWebPage(ofstream &out) = 0;

    void addHtmlLineNumbers(ofstream &out);
    void addHtmlDescription(ofstream &out);
    void addHtmlPrecondition(ofstream &out);
    void addHtmlManual(ofstream &out);

private:
    const char *getStatusName();
    string convertHtmlString(const char *html);
    void convertHtmlString(StringTokenizer &tokenizer, string line[], 
                           int &lineNum, int level);
    bool isBlankLine(string &line);
};

#endif

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

#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "Executive.h"
#include "BaseTestCase.h"
#include "Summary.h"

using namespace std;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Executive::Executive(BaseTestSuite *ts) {
    this->testSuite = ts;
    this->prompter = new UserPrompter();
}

/*****************************************************************************
 * Destructor
 *****************************************************************************/

Executive::~Executive() {
}

/*****************************************************************************
 * Execute the test cases.
 *****************************************************************************/

void Executive::execute(List &includedTests,
                        List &excludedTests,
                        bool includeManual,
                        bool appendToLogFile) {
    List tests;
    Summary summary;

    initialize();

    ofstream out;
    if (appendToLogFile) {
        out.open("log", ios::app);
    } else {
        out.open("log", ios::out);
    }

    if (!out) {
        cout << "Could not open log file.";
    } else {

        // For each test (may be a test suite), we must obtain 
        // its test cases.  We will the execute each of those
        // test cases unless they are to be excluded or they are
        // manual and we are not running the manual tests.

        ListIterator testIterator = includedTests.iterator();
        while (testIterator.hasNext()) {
            tests.clear();
            BaseTest *test = (BaseTest *) testIterator.next();
            test->getTestCases(tests);
    
            ListIterator iterator = tests.iterator();
            while (iterator.hasNext()) {
                BaseTestCase *tc = (BaseTestCase *) iterator.next();
                if (!isExcluded(tc, excludedTests)) {
                    if (!tc->isManual() || (tc->isManual() && includeManual)) {
                        tc->execute(this);
                        tc->saveReport(out);
                        tc->saveReport(cout);
                        cout.flush();
                        summary.update(tc);
                    }
                }
            }
        }
    
        int count = testSuite->getTestCaseCount();

        // Write out the summary

        cout << endl;
        summary.write(cout);
        cout << "Total test cases: " << count << endl;

        if (!appendToLogFile) {
            out << endl;
            summary.write(out);
            out << "Total test cases: " << count << endl;
        }

        cout.flush();
        out.close();
    }
}

/*****************************************************************************
 * Initialization needed to run the test cases.
 *****************************************************************************/

void Executive::initialize() {
    // do nothing here; usually overridden by derived class.
}

/*****************************************************************************
 * Has this test case been excluded?
 *****************************************************************************/

bool Executive::isExcluded(BaseTestCase *tc, List &excludedList) {
    bool exclude = false;

    ListIterator iterator = excludedList.iterator();
    while (iterator.hasNext() && !exclude) {
        BaseTest *test = (BaseTest *) iterator.next();
        exclude = tc->isChildOf(test);
    }

    return exclude;
}

/*****************************************************************************
 * Return the top-level test suite.
 *****************************************************************************/

BaseTestSuite *Executive::getTestSuite() {
    return testSuite;
}

/*****************************************************************************
 * Prompt the user.
 *****************************************************************************/

void Executive::prompt(const char *message) {
    prompter->prompt(message);
}


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

#include "Arguments.h"
#include "BaseTest.h"

using namespace std;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Arguments::Arguments(int argc, char *argv[], BaseTestSuite *testSuite) {
    action = RUN_TESTS;
    runManualTests = false;
    appendToLog = false;

    int index = parseOptions(argc, argv, testSuite);
    if (action == SHOW_LIST || action == RUN_TESTS) {
        parseTestnames(index, argc, argv, testSuite);
    }
}

/*****************************************************************************
 * Parse the options.  Each option starts with a minus sign (-).  
 *****************************************************************************/

int Arguments::parseOptions(int argc, char *argv[], BaseTestSuite *testSuite) {
    int index = 1;
    while (index < argc && action != ERROR && argv[index][0] == '-') {
        index = parseOption(index, argc, argv, testSuite);
    }
    return index;
}

/*****************************************************************************
 * Parse a single option.  The legal options are:
 *
 * -i [filename]     : include tests in the given file
 * -e [filename]     : exclude tests in the given file
 * -a                : append to log file
 * -version          : show the current test suite version
 *  -list            : list test cases
 *  -help            : show help
 *  -html            : create HTML web pages
 *****************************************************************************/

int Arguments::parseOption(int index,
                           int argc, char *argv[],
                           BaseTestSuite *testSuite) {

    if (strcmp(argv[index], "-m") == 0) {
        runManualTests = true;
    } else if (strcmp(argv[index], "-a") == 0) {
        appendToLog = true;
    } else if (strcmp(argv[index], "-i") == 0) {
        index++;
        if (index != argc) {
            fillList(includedTests, argv[index], testSuite);
        } else {
            action = ERROR;
            cout << "Missing filename." << endl;
        }
    } else if (strncmp(argv[index], "-i=", 3) == 0) {
        char *filename = (char *) (argv[index] + 3);
        fillList(includedTests, filename, testSuite);
    } else if (strcmp(argv[index], "-e") == 0) {
        index++;
        if (index != argc) {
            fillList(excludedTests, argv[index], testSuite);
        } else {
            action = ERROR;
            cout << "Missing filename." << endl;
        }
    } else if (strncmp(argv[index], "-e=", 3) == 0) {
        char *filename = (char *) (argv[index] + 3);
        fillList(excludedTests, filename, testSuite);
    } else if ((strcmp(argv[index], "-version") == 0) ||
               (strcmp(argv[index], "-v") == 0) ||
               (strcmp(argv[index], "--version") == 0)) {
        setAction(SHOW_VERSION);
    } else if ((strcmp(argv[index], "-list") == 0) ||
               (strcmp(argv[index], "-l") == 0) ||
               (strcmp(argv[index], "--list") == 0)) {
        setAction(SHOW_LIST);
    } else if ((strcmp(argv[index], "-help") == 0) ||
               (strcmp(argv[index], "-h") == 0) ||
               (strcmp(argv[index], "--help") == 0)) {
        setAction(SHOW_HELP);
        if (argc != 2) {
            action = ERROR;
            cout << "--help is not allowed with other options." << endl;
        } 
    } else if ((strcmp(argv[index], "-html") == 0) ||
               (strcmp(argv[index], "-H") == 0) ||
               (strcmp(argv[index], "--html") == 0)) {
        setAction(MAKE_WEB_PAGES);
        if (argc != 2) {
            action = ERROR;
            cout << "-html is not allowed with other options." << endl;
        } 
    } else {
        action = ERROR;
        cout << "Unknown option: " << argv[index] << endl;
    }
    index++;

    return index;
}

/*****************************************************************************
 * Set the current action to perform.  The default action is to run the
 * test cases.  Once changed, it cannot be changed again.  For example,
 * using the options "-list" and "-html" together is illegal.
 *****************************************************************************/

void Arguments::setAction(Action newAction) {
    if (this->action == RUN_TESTS) {
        this->action = newAction;
    } else {
        this->action = ERROR;
        cout << "Illegal mix of options." << endl;
        cout << "Help, Version, and List cannot be combined." << endl;
    }
}
    
/*****************************************************************************
 * Parse the test names.  
 *****************************************************************************/

void Arguments::parseTestnames(int index,
                               int argc, char *argv[],
                               BaseTestSuite *testSuite) {

    while (index < argc && action != ERROR) {
        BaseTest * test = testSuite->find(argv[index]);
        if (test == NULL) {
            action = ERROR;
            cout << "Test not found: " << argv[index] << endl;
        } else {
            includedTests.add(test);
        }
        index++;
    }

    if (includedTests.isEmpty()) {
        includedTests.add(testSuite);
    }
}

/*****************************************************************************
 * For each testname in the file, add the actual test object to the list.
 *****************************************************************************/

void Arguments::fillList(List &list,
                         const char *filename,
                         BaseTestSuite *testSuite) {

    char testname[1000];

    ifstream in(filename, ios::in);
    if (!in) {
        action = ERROR;
        cout << "File not found: " << filename << endl;
    } else {
        while (!in.eof() && action != ERROR) {
            testname[0] = '\0';
            in >> testname;
            if (strlen(testname) != 0) {
                BaseTest *test = testSuite->find(testname);
                if (test == NULL) {
                    action = ERROR;
                    cout << "Test not found: " << testname << endl;
                } else {
                    list.add(test);
                }
            }
        }
        in.close();

        if (action != ERROR && list.isEmpty()) {
            action = ERROR;
            cout << "No test names in " << filename << endl;
        }
    }
}

/*****************************************************************************
 * Return the action that is to be performed.
 *****************************************************************************/

Action Arguments::getAction() {
    return action;
}

/*****************************************************************************
 * Should the manual tests be included in the run?
 *****************************************************************************/

bool Arguments::includeManualTests() {
    return runManualTests;
}

/*****************************************************************************
 * Should the test case results be appended to the log file?
 *****************************************************************************/

bool Arguments::appendResultsToLogFile() {
    return appendToLog;
}

/*****************************************************************************
 * Return the list of tests to run.
 *****************************************************************************/

List &Arguments::getIncludedTests() {
    return includedTests;
}

/*****************************************************************************
 * Return the list of tests to exclude.
 *****************************************************************************/

List &Arguments::getExcludedTests() {
    return excludedTests;
}


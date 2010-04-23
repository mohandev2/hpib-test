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

#include "Controller.h"
#include "HtmlGenerator.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Controller::Controller(const char *ver,
                       int argc,
                       char *argv[],
                       Executive *e,
                       BaseTestSuite *ts) {
    this->version = ver;
    this->arguments = new Arguments(argc, argv, ts);
    this->executive = e;
    this->testSuite = ts;
}

/*****************************************************************************
 * Destructor
 *****************************************************************************/

Controller::~Controller() {
    delete testSuite;
    delete executive;
    delete arguments;
}

/*****************************************************************************
 * Run the controller.
 *
 * Based upon the Arguments, execute a given action.
 *****************************************************************************/

void Controller::run() {
    switch (arguments->getAction()) {
        case ERROR: 
            break;

        case SHOW_VERSION: 
            showVersion();
            break;

        case SHOW_HELP: 
            showHelp();
            break;

        case SHOW_LIST: 
            showList();
            break;

        case MAKE_WEB_PAGES: 
            makeWebPages();
            break;

        case RUN_TESTS: 
            runTests();
            break;
    }
}

/*****************************************************************************
 * Show the current test suite version.
 *****************************************************************************/

void Controller::showVersion() {
    cout << "Version " << version << endl;
}

/*****************************************************************************
 * Show help.
 *****************************************************************************/

void Controller::showHelp() {
    cout << "Usage: hpitest [OPTION]... [TESTNAME]...\n";
    cout << "\n";
    cout << "-m                   include the manual test cases\n";
    cout << "-i=FILENAME          file containing the names of the tests to include\n";
    cout << "-e=FILENAME          file containing the names of the tests to exclude\n";
    cout << "-a                   append to the log file rather than overwrite\n";
    cout << "-l, --list           list the names of all of the test cases\n";
    cout << "-H, --html           create the HTML web pages\n";
    cout << "-h, --help           show this help\n";
    cout << "-v, --version        show version\n";
    cout.flush();
}

/*****************************************************************************
 * Show the list of test cases.
 *****************************************************************************/

void Controller::showList() {
    ListIterator iterator = arguments->getIncludedTests().iterator();
    while (iterator.hasNext()) {
        BaseTest *test = (BaseTest *) iterator.next();
        test->printList();
    }
}

/*****************************************************************************
 * Make the HTML web pages.
 *****************************************************************************/

void Controller::makeWebPages() {
    HtmlGenerator generator;

    generator.generate(testSuite, version);
}

/*****************************************************************************
 * Run the test cases.
 *****************************************************************************/

void Controller::runTests() {
    executive->execute(arguments->getIncludedTests(),
                       arguments->getExcludedTests(),
                       arguments->includeManualTests(),
                       arguments->appendResultsToLogFile());
}


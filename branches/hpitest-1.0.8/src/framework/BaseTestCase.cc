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

#include <iostream>
#include <fstream>
#include <cstring>

#include "TestStatus.h"
#include "BaseTestCase.h"
#include "Timer.h"
#include "Time.h"
#include "LocationMgr.h"
#include "AppExit.h"
#include "StringTokenizer.h"

using namespace std;

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

BaseTestCase::BaseTestCase() {
    this->testCaseLine = NULL;
}

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

BaseTestCase::BaseTestCase(char *line) { 
    this->testCaseLine = line;
}

/*****************************************************************************
 * Destructor.
 *****************************************************************************/

BaseTestCase::~BaseTestCase() {
}

/*****************************************************************************
 * Fake polymorphism.
 *****************************************************************************/

bool BaseTestCase::isTestCase() {
    return true;
}

/*****************************************************************************
 * Return the test's status.
 *****************************************************************************/

TestStatus &BaseTestCase::getStatus() {
    return testCaseStatus;
}

/*****************************************************************************
 * Return Precondition.
 *****************************************************************************/

const char *BaseTestCase::getPrecondition() {
    return NULL;
}

/*****************************************************************************
 * Is this a manual test.
 *****************************************************************************/

bool BaseTestCase::isManual() {
    return false;
}

/*****************************************************************************
 * Callback function if the test times out.  The application will abort since
 * this would be an unrecoverable error.
 *****************************************************************************/

void BaseTestCase::testBlockCallback(void *data) {
    BaseTestCase *test = (BaseTestCase *) data;
    AppExit::timeoutError(test->getPath());
}

/*****************************************************************************
 * Execute the test.  Use a timer to abort if the test case blocks.
 *****************************************************************************/

void BaseTestCase::execute(Executive *e) {
    this->executive = e;

    blockId = Timer::add(testBlockCallback, getTimeout(), this);

    testCaseStatus = runTest();

    Timer::remove(blockId);
}

/*****************************************************************************
 * Pause the test timer.  The block timer is paused so that the application
 * will not abort.
 *****************************************************************************/

void BaseTestCase::pauseTimer() {
    Timer::pause(blockId);
}

/*****************************************************************************
 * Resume the test timer.
 *****************************************************************************/

void BaseTestCase::resumeTimer() {
    Timer::resume(blockId);
}

/*****************************************************************************
 * Push a Location.
 *****************************************************************************/

void BaseTestCase::pushLocation(const char *name, unsigned int id) {
    LocationMgr::push(name, id);
}

/*****************************************************************************
 * Pop a Location.
 *****************************************************************************/

void BaseTestCase::popLocation() {
    LocationMgr::pop();
}

/*****************************************************************************
 * Save the test pathname and it's status to a text file.
 *****************************************************************************/

void BaseTestCase::saveReport(ostream &out) {
    out << getPath() << ": " << testCaseStatus.getName() << endl;
    if (testCaseStatus.hasFault()) {
        out << endl;
        string result = testCaseStatus.toString();
        out << "   ";
        int len = result.size();
        for (int i = 0; i < len; i++) {
            out << result[i];
            if (result[i] == '\n' && i != len - 1) {
                out << "   ";
            }
        }
    } else if (testCaseStatus.isNotSupported()) {
        const char *precondition = getPrecondition();
        if (precondition != NULL) {
            out << endl;
            out << convertHtmlString(precondition);
            out << endl;
        }
    }
}

/*****************************************************************************
 * Convert an HTML string into a non-HTML string.
 *****************************************************************************/

string BaseTestCase::convertHtmlString(const char *html) {
    string buf;
    string line[200];
    int lineNum = 0;
    StringTokenizer tokenizer(html);
    convertHtmlString(tokenizer, line, lineNum, 0);
    for (int i = 0; i <= lineNum; i++) {
        buf.append("   ");  // indent
        buf.append(line[i]);
        buf.append("\n");
    }
    return buf;
}

/*****************************************************************************
 * Convert an HTML string into a non-HTML string.
 *****************************************************************************/

void BaseTestCase::convertHtmlString(StringTokenizer &tokenizer, string line[], 
                                     int &lineNum, int level) {
    char str[10];

    int cnt = 0;
    while (tokenizer.hasMoreTokens()) {
        string token = tokenizer.nextToken();
        if (token == "<b>") {
            // ignore
        } else if (token == "</b>") {
            // ignore
        } else if (token == "<i>") {
            // ignore
        } else if (token == "</i>") {
            // ignore
        } else if (token == "</ol>") {
            break;
        } else if (token == "</ul>") {
            break;
        } else if (token == "<ol>" || token == "<ul>") {
            convertHtmlString(tokenizer, line, lineNum, level+1);
            if (level > 0) {
                line[lineNum].append("   ");
                for (int i = 0; i < level; i++) {
                    line[lineNum].append("   ");
                }
            }
        } else if (token == "<li>") {
            if (!isBlankLine(line[lineNum])) {
                lineNum++;
                lineNum++;
            } else if (lineNum >= 1 && !isBlankLine(line[lineNum-1])) {
                lineNum++;
            }
            line[lineNum].assign("");
            for (int i = 0; i < level; i++) {
                line[lineNum].append("   ");
            }
            cnt++;
            if (level <= 1) {
                sprintf(str, "%d", cnt);
            } else {
                str[0] = 'a' + cnt - 1;
                str[1] = 0;
            } 
            line[lineNum].append(str);
            line[lineNum].append(". ");
        } else if (token == "</li>") {
            if (!isBlankLine(line[lineNum])) {
                lineNum++;
                lineNum++;
            } else if (lineNum >= 1 && !isBlankLine(line[lineNum-1])) {
                lineNum++;
            }
        } else {
            line[lineNum].append(token);
            line[lineNum].append(" ");
            if (line[lineNum].length() >= 70) {
                lineNum++;
                if (level > 0) {
                    line[lineNum].append("   ");
                    for (int i = 0; i < level; i++) {
                        line[lineNum].append("   ");
                    }
                }
            }
        }
    }
}

/*****************************************************************************
 * Is this a blank line?
 *****************************************************************************/

bool BaseTestCase::isBlankLine(string &line) {
    if (line.empty()) 
        return true;
    
    int len = line.size();
    for (int i = 0; i < len; i++) {
        if (line[i] != ' ') {
            return false;
        }
    }

    return true;
}

/*****************************************************************************
 * Add this test case to the list.
 *****************************************************************************/

void BaseTestCase::getTestCases(List &list) {
    list.add(this);
}

/*****************************************************************************
 * Is this test case the test we are trying to find?
 *****************************************************************************/

BaseTest *BaseTestCase::find(const char *name) {
    const char *mypath = getPath();
    if (mypath != NULL && strcasecmp(name, mypath) == 0) {
        return this;
    }
    return NULL;
}

/*****************************************************************************
 * Return the executive.
 *****************************************************************************/

Executive *BaseTestCase::getExecutive() {
    return executive;
}

/*****************************************************************************
 * Pause for the given number of seconds.
 *****************************************************************************/

void BaseTestCase::secSleep(unsigned int seconds) {
    Timer::sleep(seconds);
}

/*****************************************************************************
 * Pause for the given number of milliseconds.
 *****************************************************************************/

void BaseTestCase::msecSleep(long msec) {
    Timer::usecSleep(msec * 1000);
}

/*****************************************************************************
 * Pause for the given number of microseconds.
 *****************************************************************************/

void BaseTestCase::usecSleep(long usec) {
    Timer::usecSleep(usec);
}

/*****************************************************************************
 * Add to a web page.
 *****************************************************************************/

void BaseTestCase::addHtml(ofstream &out) {
    out << "<h4>" << getName() << "</h4>\n";
    fillWebPage(out);
}

/*****************************************************************************
 * Add the optional line numbers to the HTML document.
 *****************************************************************************/

void BaseTestCase::addHtmlLineNumbers(ofstream &out) {
    char tmp[200];
    if (testCaseLine != NULL) {
        strcpy(tmp, testCaseLine);

        out << "Line(s):\n";
        out << "<div class=\"indent\">\n";

        char *pch = strtok(tmp, ",; ");
        while (pch != NULL) {
            out << pch << "<br />\n";
            pch = strtok (NULL, ",; ");
        }

        out << "</div>\n";
    }
}

/*****************************************************************************
 * Add the description to the HTML document.
 *****************************************************************************/

void BaseTestCase::addHtmlDescription(ofstream &out) {
    out << "Description:\n";
    out << "<div class=\"indent\">\n";
    out << getDescription() << "\n";
    out << "</div>\n";
}

/*****************************************************************************
 * Add the optional precondition to the HTML document.
 *****************************************************************************/

void BaseTestCase::addHtmlPrecondition(ofstream &out) {
    const char *precondition = getPrecondition();
    if (precondition != NULL) {
        out << "Precondition:\n";
        out << "<div class=\"indent\">\n";
        out << precondition << "\n";
        out << "</div>\n";
    }
}

/*****************************************************************************
 * Add the manual flag to the HTML document.
 *****************************************************************************/

void BaseTestCase::addHtmlManual(ofstream &out) {
    const char * manual = isManual() ? "Yes" : "No";
    out << "Manual: " << manual << "<br />\n";
}

/*****************************************************************************
 * Count this test only.
 *****************************************************************************/

int BaseTestCase::getTestCaseCount() {
    return 1;
}

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

#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <cstring>
#include <iostream>
#include <fstream>

#include "List.h"
#include "BaseTestSuite.h"
#include "HtmlGenerator.h"

using namespace std;

/*****************************************************************************
 * Actions
 *****************************************************************************/

typedef enum {
    ERROR,
    SHOW_VERSION,
    SHOW_HELP,
    SHOW_LIST,
    MAKE_WEB_PAGES,
    RUN_TESTS
} Action;

/*****************************************************************************
 * Arguments - Parse the command line arguments.
 *****************************************************************************/

class Arguments
{
private:
    Action action;
    bool runManualTests;
    bool appendToLog;
    List includedTests;
    List excludedTests;

public:
    Arguments(int argc, char *argv[], BaseTestSuite *testSuite);

    Action getAction();

    bool includeManualTests();
    bool appendResultsToLogFile();

    List &getIncludedTests();
    List &getExcludedTests();

private:
    int parseOptions(int argc, char *argv[], BaseTestSuite *testSuite);
    int parseOption(int index, int argc, char *argv[], BaseTestSuite *testSuite);
    void parseTestnames(int index, int argc, char *argv[], BaseTestSuite *testSuite);
    void fillList(List &list, const char *filename, BaseTestSuite *testSuite);
    void setAction(Action action);
};

#endif


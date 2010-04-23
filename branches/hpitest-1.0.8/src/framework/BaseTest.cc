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
#include <cstdarg>
#include <cstring>

#include "BaseTest.h"
#include "StackTraceEntry.h"
#include "Executive.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

BaseTest::BaseTest() {
    parent = NULL;
    path = NULL;
}

/*****************************************************************************
 * Destructor.
 *****************************************************************************/

BaseTest::~BaseTest() {
}

/*****************************************************************************
 * Set the Parent Test.
 *****************************************************************************/

void BaseTest::setParent(BaseTest *test) {
    parent = test;
}

/*****************************************************************************
 * Get the test's full pathname.
 *****************************************************************************/

const char *BaseTest::getPath() {
    if (path == NULL) {
        if (parent == NULL) {
            return getName(); // don't set path yet
        } else {
            const char *parentPath = parent->getPath();
            if (parentPath == NULL) {
                return getName();
            } else {
                const char *name = getName();
                if (name != NULL) {
                    char buf[1000];
                    strcpy(buf, parentPath);
                    strcat(buf, "/");
                    strcat(buf, name);
                    path = new char[strlen(buf) + 1];
                    strcpy(path, buf);
                }
            }
        }
    } 
    return path;
}

/*****************************************************************************
 * Is the given test a parent test suite?  (Also includes this test.)
 *****************************************************************************/

bool BaseTest::isChildOf(BaseTest *test) {
    if (this == test) {
        return true;
    } else if (parent != NULL) {
        return parent->isChildOf(test);
    }
    return false;
}

/*****************************************************************************
 * List the pathname of the test.
 *****************************************************************************/

void BaseTest::printList() {
    const char *pathname = getPath();
    if (pathname != NULL) {
        std::cout << pathname << std::endl;
    }
}

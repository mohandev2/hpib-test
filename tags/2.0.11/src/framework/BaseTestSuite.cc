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
#include <stdlib.h>

#include <cstring>

#include "BaseTest.h"
#include "BaseTestSuite.h"
#include "Executive.h"
#include "ListIterator.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

BaseTestSuite::BaseTestSuite() {
}

/*****************************************************************************
 * Destructor.
 *****************************************************************************/

BaseTestSuite::~BaseTestSuite() {
}

/*****************************************************************************
 * Add a test to this Test Suite.
 *****************************************************************************/

void BaseTestSuite::add(BaseTest *child) {
    children.add(child);
    child->setParent(this);
}

/*****************************************************************************
 * Fake polmorphism.
 *****************************************************************************/

bool BaseTestSuite::isTestCase() {
    return false;
}

/*****************************************************************************
 * Return the list of child tests.
 *****************************************************************************/

List &BaseTestSuite::getChildren() {
    return children;
}

/*****************************************************************************
 * Add test cases.
 *****************************************************************************/

void BaseTestSuite::getTestCases(List &list) {
    ListIterator iterator = children.iterator();
    while (iterator.hasNext()) {
        BaseTest *test = (BaseTest *) iterator.next();
        test->getTestCases(list);
    }
}

/*****************************************************************************
 * Find the test with the given name.  It might be this test suite.
 * If not, then do the find to each child and perhaps one of them will match.
 * If nothing matches, return NULL.
 *****************************************************************************/

BaseTest *BaseTestSuite::find(const char *name) {
    const char *myname = getPath();
    if (myname != NULL && strcasecmp(name, myname) == 0) {
        return this;
    } else {
        ListIterator iterator = children.iterator();
        while (iterator.hasNext()) {
            BaseTest *child = (BaseTest *) iterator.next();
            BaseTest *test = child->find(name);
            if (test != NULL) {
                return test;
            }
        }
        return NULL;
    }
}

/*****************************************************************************
 * List the pathname of the test suite's children.
 *****************************************************************************/

void BaseTestSuite::printList() {
    ListIterator iterator = children.iterator();
    while (iterator.hasNext()) {
        BaseTest *child = (BaseTest *) iterator.next();
        child->printList();
    }
}

/*****************************************************************************
 * Make the HTML web pages.
 *****************************************************************************/

HtmlToc *BaseTestSuite::makeHtml(const char *dirname) {
    char cmd[2000];
    char pathname[2000];
    char filename[2000];
    HtmlToc *toc;

    if (children.isEmpty()) {
        toc = new HtmlToc(getName(), NULL);
    } else {
        BaseTest *childTest = (BaseTest *) children.get(0);
        if (childTest->isTestCase()) {
            sprintf(filename, "%s/TestSuite.html", dirname);
            makeWebPage(filename);
            toc = new HtmlToc(getName(), filename);
        } else {
            toc = new HtmlToc(getName(), NULL);
            ListIterator iterator = children.iterator();
            while (iterator.hasNext()) {
                BaseTestSuite *ts = (BaseTestSuite *) iterator.next();
                sprintf(pathname, "%s/%s", dirname, ts->getName());
                sprintf(cmd, "mkdir %s", pathname);
                system(cmd);
                HtmlToc *childToc = ts->makeHtml(pathname);
                toc->add(childToc);
            }
        }
    }

    return toc;
}

/*****************************************************************************
 * Make a HTML web page for a test suite that contains test cases.
 *****************************************************************************/

void BaseTestSuite::makeWebPage(const char *filename) {
    ofstream out;
    out.open(filename);
    if (!out) {
        cout << "Cannot open file " << filename << endl;
    } else {
        out << "<html>\n";
        out << "<head>\n";
        out << "<title>\n";
        out << getPath() << "\n";
        out << "</title>\n";
        out << "<style type=\"text/css\">\n";
        out << "<!--\n";
        out << "div.indent { margin-left:2em }\n";
        out << "body { color:#000000; background-color:#FFFFFF }\n";
        out << "div.small { font-size: 8pt }\n";
        out << "-->\n";
        out << "</style>\n";
        out << "</head>\n";
        out << "<body>\n";
        out << "<h2>" << getPath() << "</h2>\n";

        ListIterator iterator = children.iterator();
        while (iterator.hasNext()) {
            BaseTestCase *tc = (BaseTestCase *) iterator.next();
            tc->addHtml(out);
            out << "<hr />\n";
        }

        // UNH copyright
        out << "<br />\n";
        out << "<div align=\"center\" class=\"small\">\n";
        out << "<nobr>Copyright 2006, The University of New Hampshire, Durham, NH 03824</nobr><br />\n";
        out << "<nobr>UNH is part of the University System of New Hampshire.</nobr>\n";
        out << "</div>\n";

        out << "</body>\n";
        out << "</html>\n";
        out.close();
    }
}

/*****************************************************************************
 * Return the number of test cases in this test suite.
 *****************************************************************************/

int BaseTestSuite::getTestCaseCount() {
    int count = 0;
    ListIterator iterator = children.iterator();
    while (iterator.hasNext()) {
        BaseTest *test = (BaseTest *) iterator.next();
        count += test->getTestCaseCount();
    }

    return count;
}

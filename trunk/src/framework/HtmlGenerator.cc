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
#include <stdlib.h>
#include <unistd.h>
#include "HtmlGenerator.h"
#include "HtmlToc.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define HTML_DIRNAME "html"
#define MAX_DIR_PATHNAME 2000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HtmlGenerator::HtmlGenerator() {
}

/*****************************************************************************
 * Generate the HTML files.
 *****************************************************************************/

void HtmlGenerator::generate(BaseTestSuite *testSuite, const char *version) {

    system("rm -rf html");
    system("mkdir html");

    chdir("html");

    // Have the test suite generate all of its
    // test case web pages and return the overall
    // table of contents.

    HtmlToc *toc = testSuite->makeHtml(".");

    makeIndex(testSuite);
    makeToc(toc, version);
    makeBanner(testSuite);

    chdir("..");

    delete toc;
}

/*****************************************************************************
 * Make the index.html file.  This will be a frameset.
 *****************************************************************************/

void HtmlGenerator::makeIndex(BaseTestSuite *testSuite) {
    ofstream out;
    out.open("index.html", ios::out);
    if (!out) {
        cout << "Cannot open file: index.html" << endl;
    } else {
        out << "<html>\n";
        out << "<head>\n";
        out << "<title>" << testSuite->getTitle() << "</title>\n";
        out << "</head>\n";
        out << "<frameset rows=\"130,*\">\n";
        out << "<frame src=\"banner.html\" noresize=\"noresize\">\n";
        out << "<frameset cols=\"40%,*\">\n";
        out << "<frame src=\"toc.html\">\n";
        out << "<frame name=\"test\">\n";
        out << "</frameset>\n";
        out << "<noframes>\n";
        out << "This is a frame-based document.\n";
        out << "</noframes>\n";
        out << "</frameset>\n";
        out << "</html>\n";
        out.flush();
        out.close();
    }
}

/*****************************************************************************
 * Make the Table of Contents HTML file.
 *****************************************************************************/

void HtmlGenerator::makeToc(HtmlToc *toc, const char *version) {
    ofstream out;
    out.open("toc.html", ios::out);
    if (!out) {
        cout << "Cannot open file: toc.html" << endl;
    } else {
        out << "<html>\n";
        out << "<head>\n";
        out << "<title>\n";
        out << "Table of contents\n";
        out << "</title>\n";
        out << "<style type=\"text/css\">\n";
        out << "<!--\n";
        out << "body { color:#000000; background-color:#FFFBD3 }\n";
        out << "a:link { color:black }\n";
        out << "a:active { color:red }\n";
        out << "a:visited { color:black }\n";
        out << "-->\n";
        out << "</style>\n";
        out << "</head>\n";
        out << "<body>\n";
        out << "<h3>Test Suites v" << version << "</h3>\n";
        toc->toHtml(out);
        out << "</body>\n";
        out << "</html>\n";
        out.flush();
        out.close();
    }
}

/*****************************************************************************
 * Make the Banner HTML file.
 *****************************************************************************/

void HtmlGenerator::makeBanner(BaseTestSuite *testSuite) {
    ofstream out;
    out.open("banner.html", ios::out);
    if (!out) {
        cout << "Cannot open file: banner.html" << endl;
    } else {
        out << "<html>\n";
        out << "<head>\n";
        out << "<style type=\"text/css\">\n";
        out << "<!--\n";
        out << "body { color:#FFFFFF; background-color:#00008B }\n";
        out << "-->\n";
        out << "</style>\n";
        out << "</head>\n";
        out << "<body>\n";
        out << "<h2 align=\"center\">" << testSuite->getBanner() << "</h2>\n";
        out << "<h3 align=\"center\">University of New Hampshire</h3>\n";
        out << "</body>\n";
        out << "</html>\n";
        out.flush();
        out.close();
    }
}

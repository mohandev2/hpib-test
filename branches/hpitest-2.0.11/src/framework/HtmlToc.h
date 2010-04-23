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

#ifndef __HTMLTOC_H__
#define __HTMLTOC_H__

#include <iostream>
#include <fstream>

#include "List.h"

using namespace std;

/*****************************************************************************
 * An HTML Toc represents a table of content for the test suites.  Each
 * entry can have a name and an optional hyperlink.  It can also contain
 * child HTML Tocs.
 *****************************************************************************/

class HtmlToc
{
private:
    char name[300];
    char href[2000];
    List children;

public:
    HtmlToc(const char *name, const char *href);
    ~HtmlToc();

    void add(HtmlToc *toc);
    void toHtml(ofstream &out);

private:
    bool isEmpty(char *str);
};

#endif

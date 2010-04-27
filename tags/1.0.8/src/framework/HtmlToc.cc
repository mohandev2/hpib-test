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
#include <string.h>

#include "HtmlToc.h"
#include "ListIterator.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HtmlToc::HtmlToc(const char *nameValue, const char *hrefValue) {
    if (nameValue == NULL) {
        strcpy(this->name, "");
    } else {
        strcpy(this->name, nameValue);
    }

    if (hrefValue == NULL) {
        strcpy(this->href, "");
    } else {
        strcpy(this->href, hrefValue);
    }
}

/*****************************************************************************
 * Destructor
 *****************************************************************************/

HtmlToc::~HtmlToc() {
    ListIterator iterator = children.iterator();
    while(iterator.hasNext()) {
        HtmlToc *child = (HtmlToc *) iterator.next();
        delete child;
    }
}

/*****************************************************************************
 * Add a child HTML Toc.
 *****************************************************************************/

void HtmlToc::add(HtmlToc *toc) {
    children.add(toc);
}

/*****************************************************************************
 * Convert to HTML format.  Format using an HTML list.
 *****************************************************************************/

void HtmlToc::toHtml(ofstream &out) {

    // If this Toc doesn't have a name, then do nothing.
    // If it has a name, it may or may not have an associated
    // hyperlink.

    if (!isEmpty(name)) {
        if (isEmpty(href)) {
            out << name << "\n";
        } else {
            out << "<a href=\"" << href 
                << "\" target=\"test\">" << name << "</a>\n";
        }
    }

    // Add each child as a sub-list.

    if (!children.isEmpty()) {
        out << "<ul>\n";
        ListIterator iterator = children.iterator();
        while(iterator.hasNext()) {
            HtmlToc *child = (HtmlToc *) iterator.next();
            out << "<li>\n";
            child->toHtml(out);
            out << "</li>\n";
        }
        out << "</ul>\n";
    }
}

/*****************************************************************************
 * Is the string empty, i.e. a length of zero?
 *****************************************************************************/

bool HtmlToc::isEmpty(char *str) {
    return (strlen(str) == 0);
}

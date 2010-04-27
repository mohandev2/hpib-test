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
#include "StackTraceEntry.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

StackTraceEntry::StackTraceEntry(const char *funcName, const char *filename, int lineno) {
    this->func = funcName;
    this->fname = filename;
    this->line = lineno;
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

StackTraceEntry::StackTraceEntry(const StackTraceEntry &ste) {
    this->func = ste.func;
    this->fname = ste.fname;
    this->line = ste.line;
}

/*****************************************************************************
 * Return the name of the function, i.e. method.
 *****************************************************************************/

const char *StackTraceEntry::getFunction() {
    return func;
}

/*****************************************************************************
 * Return the source filename.
 *****************************************************************************/

const char *StackTraceEntry::getFilename() {
    return fname;
}

/*****************************************************************************
 * Return the line number in the source file.
 *****************************************************************************/

int StackTraceEntry::getLine() {
    return line;
}

/*****************************************************************************
 * Convert to string format.
 *****************************************************************************/

string StackTraceEntry::toString()
{
    string buf;

    buf.append("    at ");
    buf.append(func);
    buf.append(" (");
    buf.append(fname);
    buf.append(":");

    char s[100];
    sprintf(s, "%d", line);
    buf.append(s);

    buf.append(")");

    return buf;
}

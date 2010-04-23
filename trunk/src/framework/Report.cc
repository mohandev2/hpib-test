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

#include "Report.h"

using namespace std;

/*****************************************************************************
 * Constructor.
 *
 * The line is used to determine when a newline needs to be inserted, i.e.
 * no newline is needed for the first line.
 *****************************************************************************/

Report::Report() {
    line = 0;
}

/*****************************************************************************
 * Clear the report.
 *****************************************************************************/

void Report::clear() {
    buffer.clear();
    line = 0;
}

/*****************************************************************************
 * Add a message to the report.
 *****************************************************************************/

void Report::add(const char *fmt, ...) {
    char message[1000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    append(message);
}

/*****************************************************************************
 * Append a message to the Report.  Add a newline if necessary.
 *****************************************************************************/

void Report::append(const char *message) {
    if (line != 0) {
        buffer.append("\n");
    }
    buffer.append(message);
    line++;
}

/*****************************************************************************
 * Return the Report in string format.
 *****************************************************************************/

const char *Report::toString() {
    return buffer.c_str();
}

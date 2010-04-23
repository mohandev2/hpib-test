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

#ifndef __STACKTRACEENTRY_H__
#define __STACKTRACEENTRY_H__

#include <string>

using namespace std;

/*****************************************************************************
 * Stack Trace Entry
 *
 * Each entry consists of the source filename, the method (function) in the
 * source file, and the line number in the source file.
 *****************************************************************************/

class StackTraceEntry
{
private:
    const char *func;
    const char *fname;
    int line;

public:
    StackTraceEntry(const char *func, const char *fname, int line);
    StackTraceEntry(const StackTraceEntry &ste);

    const char *getFunction();
    const char *getFilename();
    int getLine();

    string toString();
};

#endif

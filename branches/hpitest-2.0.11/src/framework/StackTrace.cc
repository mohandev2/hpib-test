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

#include "StackTrace.h"
#include "StackTraceEntry.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

StackTrace::StackTrace() {
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

StackTrace::StackTrace(const StackTrace &stackTrace) {
    ListIterator iter = stackTrace.stack.iterator();
    while (iter.hasNext()) {
        StackTraceEntry *ste = (StackTraceEntry *) iter.next();
        add(new StackTraceEntry(*ste));
    }
}

/*****************************************************************************
 * Destructor.  
 *
 * Delete all of the Stack Trace entries.
 *****************************************************************************/

StackTrace::~StackTrace() {
    ListIterator iter = stack.iterator();
    while (iter.hasNext()) {
        StackTraceEntry *ste = (StackTraceEntry *) iter.next();
        delete ste;
    }
}

/*****************************************************************************
 * Add a stack trace entry.
 *****************************************************************************/

void StackTrace::add(StackTraceEntry *entry) {
    stack.add(entry);
}

/*****************************************************************************
 * Are there any stack trace entries?
 *****************************************************************************/

bool StackTrace::isEmpty() {
    return stack.isEmpty();
}

/*****************************************************************************
 * Return an iterator to traverse the stack trace entries.
 *****************************************************************************/

ListIterator StackTrace::iterator() const {
    return stack.iterator();
}

/*****************************************************************************
 * Return the stack trace entries in string format. 
 * Each entry is on a separate line.
 *****************************************************************************/

string StackTrace::toString() {
    string buf;

    ListIterator iter = stack.iterator();
    while (iter.hasNext()) {
        StackTraceEntry *ste = (StackTraceEntry *) iter.next();
        buf.append(ste->toString());
        buf.append("\n");
    }

    return buf;
}


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

#include "StatusMessage.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

StatusMessage::StatusMessage(const char *msg) {
    this->message = msg;
}

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

StatusMessage::StatusMessage(const string &msg) {
    this->message = msg;
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

StatusMessage::StatusMessage(const StatusMessage &statusMessage) {
    this->message = statusMessage.message;
    ListIterator iterator = statusMessage.stackTrace.iterator();
    while (iterator.hasNext()) {
        StackTraceEntry *ste = (StackTraceEntry *) iterator.next();
        stackTrace.add(new StackTraceEntry(*ste));
    }
}

/*****************************************************************************
 * Add a stack trace entry to the stack trace.
 *****************************************************************************/

void StatusMessage::addTrace(StackTraceEntry *ste) {
    stackTrace.add(ste);
}

/*****************************************************************************
 * Convert to string format.
 *****************************************************************************/

string StatusMessage::toString() {
    string buf;

    if (message.length() > 0) {
        buf.append(message);
    }

    if (!stackTrace.isEmpty()) {
        if (message.length() > 0) {
            buf.append("\n");
        }
        buf.append(stackTrace.toString());
        buf.append("\n");
    }

    return buf;
}

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

#ifndef __STATUSMESSAGE_H__
#define __STATUSMESSAGE_H__

#include <string>
#include "StackTrace.h"
#include "StackTraceEntry.h"

using namespace std;

/*****************************************************************************
 * Status Message
 *
 * A Status Message contains information related to the execution of a test
 * case.  These message correspond to errors and/or failures that have 
 * occured.  Along with a textual message, a stack trace is also kept so that
 * a user can go back to the original source code if necessary.
 *****************************************************************************/

class StatusMessage
{
private:
    string message;
    StackTrace stackTrace;

public:
    StatusMessage(const char *msg);
    StatusMessage(const string &msg);
    StatusMessage(const StatusMessage &statusMessage);

    void addTrace(StackTraceEntry *ste);

    string toString();
};

#endif

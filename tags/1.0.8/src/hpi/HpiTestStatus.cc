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

#include "HpiTestStatus.h"
#include "HpiString.h"
#include "LocationMgr.h"

#define BUFLEN 1000

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

HpiTestStatus::HpiTestStatus() {
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

HpiTestStatus::HpiTestStatus(const HpiTestStatus &status) : TestStatus(status) {
}

/*****************************************************************************
 * Assignment statement.
 *****************************************************************************/

HpiTestStatus& HpiTestStatus::operator=(const TestStatus& status) {
    TestStatus::operator=(status);
    return *this;
}

/*****************************************************************************
 * Assignment statement.
 *****************************************************************************/

HpiTestStatus& HpiTestStatus::operator=(const HpiTestStatus& status) {
    TestStatus::operator=(status);
    return *this;
}

/*****************************************************************************
 * Assert an error for a HPI function.
 *****************************************************************************/

void HpiTestStatus::assertError(const char *func, const char *fname, int line,
                                HpiFunctionId functionId,
                                SaErrorT expected, SaErrorT error) {
    assertError();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected));
    buf.append(".");
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Assert an error for a HPI function with an optional message.
 *****************************************************************************/

void HpiTestStatus::assertError(const char *func, const char *fname, int line,
                                HpiFunctionId functionId, SaErrorT expected,
                                SaErrorT error, const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    assertError();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected));
    buf.append(".\n");
    buf.append(message);
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Assert an error for a HPI function with two expected error codes.
 *****************************************************************************/

void HpiTestStatus::assertError(const char *func, const char *fname, int line,
                                HpiFunctionId functionId, SaErrorT expected1, 
                                SaErrorT expected2, SaErrorT error) {
    assertError();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected1));
    buf.append(" or ");
    buf.append(HpiString::error(expected2));
    buf.append(".");
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Assert an error for a HPI function with two expected error codes
 * and with an optional message.
 *****************************************************************************/

void HpiTestStatus::assertError(const char *func, const char *fname, int line,
                                HpiFunctionId functionId, SaErrorT expected1, 
                                SaErrorT expected2, SaErrorT error,
                                const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    assertError();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected1));
    buf.append(" or ");
    buf.append(HpiString::error(expected2));
    buf.append(".\n");
    buf.append(message);
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Check for an error.  This is a convienence function typically used 
 * within the cleanup section of a test case.
 *****************************************************************************/

void HpiTestStatus::checkError(const char *func, const char *fname, int line,
                               HpiFunctionId functionId, SaErrorT error) {
    if (error != SA_OK) {
        assertError(func, fname, line, functionId, SA_OK, error);
    }
}

/*****************************************************************************
 * Check for an error.  This is a convienence function typically used 
 * within the cleanup section of a test case.
 *****************************************************************************/

void HpiTestStatus::checkError(const char *func, const char *fname, int line,
                               HpiFunctionId functionId, SaErrorT error,
                               const char *fmt, ...) {
    if (error != SA_OK) {
        char message[BUFLEN];
        va_list ap;

        va_start(ap, fmt);
        vsprintf(message, fmt, ap);
        va_end(ap);

        assertError(func, fname, line, functionId, SA_OK, error, message);
    }
}

/*****************************************************************************
 * Assert a failure for an HPI function.
 *****************************************************************************/

void HpiTestStatus::assertFailure(const char *func, const char *fname, int line,
                                  HpiFunctionId functionId,
                                  SaErrorT expected, SaErrorT error) {
    assertFailure();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected));
    buf.append(".");
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Assert a failure for an HPI function with an optional message.
 *****************************************************************************/

void HpiTestStatus::assertFailure(const char *func, const char *fname, int line,
                                  HpiFunctionId functionId, SaErrorT expected,
                                  SaErrorT error, const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    assertFailure();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected));
    buf.append(".\n");
    buf.append(message);
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Assert a failure for an HPI function with two expected error code.
 *****************************************************************************/

void HpiTestStatus::assertFailure(const char *func, const char *fname, int line,
                                  HpiFunctionId functionId, SaErrorT expected1, 
                                  SaErrorT expected2, SaErrorT error) {
    assertFailure();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected1));
    buf.append(" or ");
    buf.append(HpiString::error(expected2));
    buf.append(".");
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}

/*****************************************************************************
 * Assert a failure for an HPI function with two expected error code
 * and with an optional message.
 *****************************************************************************/

void HpiTestStatus::assertFailure(const char *func, const char *fname, int line,
                                  HpiFunctionId functionId, SaErrorT expected1, 
                                  SaErrorT expected2, SaErrorT error,
                                  const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    assertFailure();
    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(HpiString::function(functionId));
    buf.append(" unexpectedly returned ");
    buf.append(HpiString::error(error));
    buf.append(" instead of ");
    buf.append(HpiString::error(expected1));
    buf.append(" or ");
    buf.append(HpiString::error(expected2));
    buf.append(".\n");
    buf.append(message);
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    addMessage(statusMessage);
}


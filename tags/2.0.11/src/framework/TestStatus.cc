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

#include "TestStatus.h"
#include "LocationMgr.h"

#define BUFLEN 1000

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

TestStatus::TestStatus() {
    state = UNKNOWN;
}

/*****************************************************************************
 * Copy Constructor.
 *
 * Copy the state and all of the status messages.
 *****************************************************************************/

TestStatus::TestStatus(const TestStatus &status) {
    state = status.state;

    ListIterator iter = status.messageList.iterator();
    while (iter.hasNext()) {
        StatusMessage *message = (StatusMessage *) iter.next();
        messageList.add(new StatusMessage(*message));
    }
}

/*****************************************************************************
 * Destructor.
 *****************************************************************************/

TestStatus::~TestStatus() {
    free();
}

/*****************************************************************************
 * Assignment Operator.
 *
 * Copy the state and all of the status messages.
 *****************************************************************************/

TestStatus& TestStatus::operator=(const TestStatus& status) {
    state = status.state;
    free();

    ListIterator iter = status.messageList.iterator();
    while (iter.hasNext()) {
        StatusMessage *message = (StatusMessage *) iter.next();
        messageList.add(new StatusMessage(*message));
    }

    return *this;
}

/*****************************************************************************
 * Free all of the status messages.
 *****************************************************************************/

void TestStatus::free() {
    ListIterator iter = messageList.iterator();
    while (iter.hasNext()) {
        StatusMessage *message = (StatusMessage *) iter.next();
        delete message;
    }
    messageList.clear();
}

/*****************************************************************************
 * Return a iterator to traverse the status messages.
 *****************************************************************************/

ListIterator TestStatus::iterator() {
    return messageList.iterator();
}

/*****************************************************************************
 * Set the state to PASS.  According the precedence rules, the state
 * can only be set to PASS if the state is currently UNKNOWN or NOT_SUPPORT.
 *****************************************************************************/

void TestStatus::assertPass() {
    if (state == UNKNOWN || state == NOT_SUPPORT) {
        state = PASS;
    }
}

/*****************************************************************************
 * Set the state to PASS WARN.  According the precedence rules, the state
 * can only be set to PASS WARN if the state is currently UNKNOWN, NOT_SUPPORT,
 * or PASS.
 *****************************************************************************/

void TestStatus::assertPassWarn() {
    if (state == UNKNOWN || state == NOT_SUPPORT || state == PASS) {
        state = PASS_WARN;
    }
}

/*****************************************************************************
 * Add an Error.
 *****************************************************************************/

void TestStatus::assertPassWarn(const char *func, const char *fname,
                                int line, const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(message);

    assertPassWarn();
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    messageList.add(statusMessage);
}

/*****************************************************************************
 * Set the state to ERROR.  According the precedence rules, the state
 * can only be set to ERROR if the state is not currently FAIL.
 *****************************************************************************/

void TestStatus::assertError() {
    if (state == UNKNOWN || state == NOT_SUPPORT || state == PASS || state == PASS_WARN) {
        state = ERROR;
    }
}

/*****************************************************************************
 * Set the state to FAIL. 
 *****************************************************************************/

void TestStatus::assertFailure() {
    if (state == UNKNOWN || state == NOT_SUPPORT || 
        state == PASS || state == PASS_WARN || state == ERROR) {

        state = FAIL;
    }
}

/*****************************************************************************
 * Set the state to BLOCK.  Once in the BLOCK state, the state cannot be
 * changed.
 *****************************************************************************/

void TestStatus::assertBlock() {
       state = BLOCK;
}

/*****************************************************************************
 * Set the state to NOT_SUPPORT.  According the precedence rules, the state
 * can only be set to NOT_SUPPORT if the state is currently UNKNOWN.
 *****************************************************************************/

void TestStatus::assertNotSupport() {
    if (state == UNKNOWN) {
        state = NOT_SUPPORT;
    }
}

/*****************************************************************************
 * Add an error.  
 *****************************************************************************/

void TestStatus::assertError(const char *message) {
    assertError();
    messageList.add(new StatusMessage(message));
}

/*****************************************************************************
 * Add an Error.
 *****************************************************************************/

void TestStatus::assertError(const char *func, const char *fname,
                             int line, const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(message);

    assertError();
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    messageList.add(statusMessage);
}

/*****************************************************************************
 * Assert a failure.
 *****************************************************************************/

void TestStatus::assertFailure(const char *message) {
    assertFailure();
    addMessage(new StatusMessage(message));
}

/*****************************************************************************
 * Add a Failure.
 *****************************************************************************/

void TestStatus::assertFailure(const char *func, const char *fname,
                               int line, const char *fmt, ...) {
    char message[BUFLEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(message, fmt, ap);
    va_end(ap);

    string buf;
    buf.append(LocationMgr::getLocation());
    buf.append(message);

    assertFailure();
    StatusMessage *statusMessage = new StatusMessage(buf);
    statusMessage->addTrace(new StackTraceEntry(func, fname, line));
    messageList.add(statusMessage);
}

/*****************************************************************************
 * Add a TestStatus into this TestStatus.
 *****************************************************************************/

void TestStatus::add(const char *func, const char *fname, int line, const TestStatus &status) {

    if (status.state == BLOCK) {
        assertBlock();
    } else if (status.state == FAIL) {
        assertFailure();
    } else if (status.state == ERROR) {
        assertError();
    } else if (status.state == PASS) {
        assertPass();
    } else if (status.state == PASS_WARN) {
        assertPassWarn();
    } else if (status.state == NOT_SUPPORT) {
        assertNotSupport();
    }

    ListIterator iter = status.messageList.iterator();
    while (iter.hasNext()) {
        StatusMessage *message = (StatusMessage *) iter.next();
        StatusMessage *newMessage = new StatusMessage(*message);
        newMessage->addTrace(new StackTraceEntry(func, fname, line));
        messageList.add(newMessage);
    }
}

/*****************************************************************************
 * Merge a TestStatus into this TestStatus.
 *****************************************************************************/

void TestStatus::merge(const TestStatus &status) {

    if (status.state == BLOCK) {
        assertBlock();
    } else if (status.state == FAIL) {
        assertFailure();
    } else if (status.state == ERROR) {
        assertError();
    } else if (status.state == PASS) {
        assertPass();
    } else if (status.state == PASS_WARN) {
        assertPassWarn();
    } else if (status.state == NOT_SUPPORT) {
        assertNotSupport();
    } 

    ListIterator iter = status.messageList.iterator();
    while (iter.hasNext()) {
        StatusMessage *message = (StatusMessage *) iter.next();
        messageList.add(new StatusMessage(*message));
    }
}

/*****************************************************************************
 * Is the status unknown?
 *****************************************************************************/

bool TestStatus::isUnknown() {
    return (state == UNKNOWN);
}

/*****************************************************************************
 * Is the status okay?
 *****************************************************************************/

bool TestStatus::isOkay() {
    return (state == UNKNOWN || state == PASS);
}

/*****************************************************************************
 * Is the test blocked?
 *****************************************************************************/

bool TestStatus::isBlocked() {
    return (state == BLOCK);
}

/*****************************************************************************
 * Has the test passed?
 *****************************************************************************/

bool TestStatus::isPassed() {
    return (state == PASS || state == PASS_WARN);
}

/*****************************************************************************
 * Has the test passed with a warning?
 *****************************************************************************/

bool TestStatus::hasWarning() {
    return (state == PASS_WARN);
}

/*****************************************************************************
 * Has the test encountered an error?
 *****************************************************************************/

bool TestStatus::hasError() {
    return (state == ERROR);
}

/*****************************************************************************
 * Has the test encountered a failure?
 *****************************************************************************/

bool TestStatus::hasFailure() {
    return (state == FAIL);
}

/*****************************************************************************
 * Has the test encountered an error, failure, or block?
 *****************************************************************************/

bool TestStatus::hasFault() {
    return hasError() || hasFailure() || isBlocked();
}

/*****************************************************************************
 * Is the test not supported?
 *****************************************************************************/

bool TestStatus::isNotSupported() {
    return (state == NOT_SUPPORT);
}

/*****************************************************************************
 * Return the status name.
 *****************************************************************************/

const char *TestStatus::getName() {
    if (isBlocked()) {
        return "BLOCK";
    } else if (hasFailure()) {
        return "FAIL";
    } else if (hasError()) {
        return "ERROR";
    } else if (isNotSupported()) {
        return "NOTSUPPORT";
    } else if (isPassed()) {
        if (hasWarning()) {
            return "PASS_WARN";
        } else {
            return "PASS";
        }
    } else {
        return "UNKNOWN";
    }
}

/*****************************************************************************
 * Convert to string format.
 *****************************************************************************/

string TestStatus::toString() {
    string buf;

    ListIterator iter = messageList.iterator();
    while (iter.hasNext()) {
        StatusMessage *message = (StatusMessage *) iter.next();
        string s = message->toString();
        if (s.length() > 0) {
            buf.append(s);
            buf.append("\n");
        }
    }

    return buf;
}

/*****************************************************************************
 * Add a status message.
 *****************************************************************************/

void TestStatus::addMessage(StatusMessage *message) {
    messageList.add(new StatusMessage(*message));
}

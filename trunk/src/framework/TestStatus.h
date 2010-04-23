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

#ifndef __TESTSTATUS_H__
#define __TESTSTATUS_H__

#include <string>
#include "List.h"
#include "ListIterator.h"
#include "StatusMessage.h"

using namespace std;

/*****************************************************************************
 * When adding error and failure messages, users are required to include
 * a TRACE of so that a stack trace can be recorded and shown to a user.
 *****************************************************************************/

#define TRACE __func__, __FILE__, __LINE__

/*****************************************************************************
 * TestStatus
 *
 * Initially, every TestStatus starts in the UKNOWN state.  It is the 
 * responsibility of the actual test to change the state to NOT_SUPPORT,
 * ERROR, or FAIL.  After a test has finished executing, if the state is
 * still UNKNOWN, it is changed to PASS.
 *****************************************************************************/

class TestStatus
{
private:
    typedef enum {
        UNKNOWN,
        NOT_SUPPORT,
        PASS,
        PASS_WARN,
        ERROR,
        FAIL, 
        BLOCK 
    } TestState;

private:
    TestState state;
    List messageList;

public:
    TestStatus();
    TestStatus(const TestStatus &status);
    virtual ~TestStatus();

    virtual TestStatus& operator=(const TestStatus &status);

    ListIterator iterator();

    void assertBlock();
    void assertPass();
    void assertPassWarn();
    void assertError();
    void assertFailure();
    void assertNotSupport();

    void assertPassWarn(const char *func, const char *fname, int line, const char *fmt, ...);

    void assertError(const char *message);

    void assertError(const char *func, const char *fname, int line, const char *fmt, ...);

    virtual void assertFailure(const char *message);

    void assertFailure(const char *func, const char *fname, int line, const char *fmt, ...);

    void add(const char *func, const char *fname, int line, const TestStatus &status);

    void merge(const TestStatus &status);

    bool isUnknown();
    bool isOkay();
    bool isBlocked();
    bool isPassed();
    bool hasWarning();
    bool hasError();
    bool hasFailure();
    bool isNotSupported();
    bool hasFault(); // error or failure

    const char *getName();

    string toString();

private:
    void free();

protected:
    void addMessage(StatusMessage *message);

};

#endif

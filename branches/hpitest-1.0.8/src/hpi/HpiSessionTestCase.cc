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

#include "TestStatus.h"
#include "HpiSessionTestCase.h"
#include "LocationMgr.h"
#include "Timer.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiSessionTestCase::HpiSessionTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiSessionTestCase::HpiSessionTestCase(char *line) : HpiTestCase(line) { 
}

/*****************************************************************************
 * Callback function if the test times out.  We will try to force the HPI
 * implementation to return control to the application by closing the session.
 *****************************************************************************/

void HpiSessionTestCase::testBlockCallback(void *data) {
    HpiSessionTestCase *test = (HpiSessionTestCase *) data;
    test->closeSession();
}

/*****************************************************************************
 * Close the session and indicate the a block has occurred.
 *****************************************************************************/

void HpiSessionTestCase::closeSession() {
    isBlocked = true;
    saHpiSessionClose(testSessionId);
}

/*****************************************************************************
 * Run the HPI Session Test Case.
 *
 * Test each session.
 *
 * Note that we can only timeout if we are using threads.  We can only
 * call HPI functions from other threads, not signal callbacks.
 *****************************************************************************/

TestStatus HpiSessionTestCase::runTest() {
    HpiTestStatus status;

    isBlocked = false;
#ifdef USE_THREADS
    blockId = Timer::add(testBlockCallback, getSessionTimeout(), this);
#endif

    IdList &domainIdList = getDomainIdList();
    for (int i = 0; i < domainIdList.getCount() && !status.hasFault(); i++) {
        SaHpiDomainIdT domainId = domainIdList.getId(i);
        status.merge(testSession(domainId));

        if (isBlocked) {
            status.assertBlock();
        }
    }
#ifdef USE_THREADS
    Timer::remove(blockId);
#endif

    return status;
}

/*****************************************************************************
 * Test a session.
 *****************************************************************************/

HpiTestStatus HpiSessionTestCase::testSession(SaHpiDomainIdT domainId) {
    HpiTestStatus status;

    LocationMgr::push("Domain", domainId);

    SaErrorT error = saHpiSessionOpen(domainId, &testSessionId, NULL);
    if (error != SA_OK) {
        status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
    } else {
        if (isForceDiscovery()) {
            // The OpenHPI simulator requires that we
            // do a discovery after openning a session.
            saHpiDiscover(testSessionId); 
        }

        status = runSessionTest(testSessionId);

        if (!isBlocked) {
            error = saHpiSessionClose(testSessionId);
            status.checkError(TRACE, SESSION_CLOSE, error);
        }
    }

    LocationMgr::pop();

    return status;
}

/*****************************************************************************
 * Return the timeout for running a session test.  A "game" is being played 
 * here.  If a test case blocks, we want to try to unblock it so that we can 
 * still run the other test cases.  We will try to do that by closing the 
 * session.  Hopefully, the closing of the session will cause the HPI 
 * implementation to return control to the application.  We will give the 
 * HPI implementation SESSION_TIMEOUT_OFFSET seconds to return control.  If 
 * it doesn't, then the timeout mechanism in the Framework will take over and 
 * cause the application to exit.
 *****************************************************************************/

int HpiSessionTestCase::getSessionTimeout() {
    return HpiTestCase::getTimeout() - SESSION_TIMEOUT_OFFSET;
}

/*****************************************************************************
 * Pause this block timer and the Framework timer. 
 *****************************************************************************/

void HpiSessionTestCase::pauseTimer() {
    HpiTestCase::pauseTimer();
    Timer::pause(blockId);
}

/*****************************************************************************
 * Resume this block timer and the Framework timer. 
 *****************************************************************************/

void HpiSessionTestCase::resumeTimer() {
    HpiTestCase::resumeTimer();
    Timer::resume(blockId);
}

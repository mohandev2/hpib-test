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

#include "GetTimer.h"
#include "Report.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetTimer::GetTimer(char *line) : WatchdogTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetTimer::getName() {
    return "GetTimer";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetTimer::getDescription() {
    return "Retrieve the watchdog timer settings and configuration and\n"
           "verify that the values are legal.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetTimer::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetTimer::runWatchdogTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiWatchdogRecT *watchdogRec) {
    HpiTestStatus status;
    Report report;
    SaHpiWatchdogT watchdog;

    SaErrorT error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                           watchdogRec->WatchdogNum, &watchdog);
    if (error != SA_OK) {
        status.assertFailure(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
    } else if (!WatchdogHelper::isValid(&watchdog, report)) {
        status.assertFailure(TRACE, report.toString());
    } else {
        status.assertPass();
    }

    return status;
}

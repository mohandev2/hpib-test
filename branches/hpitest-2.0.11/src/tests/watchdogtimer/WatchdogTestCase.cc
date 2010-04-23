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

#include "WatchdogTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WatchdogTestCase::WatchdogTestCase(bool isRunOK)
: HpiRdrTestCase(SAHPI_CAPABILITY_WATCHDOG) {
    this->isRunningOK = isRunOK;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WatchdogTestCase::WatchdogTestCase(char *line, bool isRunOK)
: HpiRdrTestCase(line, SAHPI_CAPABILITY_WATCHDOG) {
    this->isRunningOK = isRunOK;
}

/*****************************************************************************
 * Run the main test only if this is a Watchdog Timer.
 *****************************************************************************/

HpiTestStatus WatchdogTestCase::runRdrTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiRdrT *rdr) {
    HpiTestStatus status;

    if (rdr->RdrType != SAHPI_WATCHDOG_RDR) {
        status.assertNotSupport();
    } else {
        pushLocation("Watchdog", rdr->RdrTypeUnion.WatchdogRec.WatchdogNum);

        if (isRunningOK) {
            status.add(TRACE, runWatchdogTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.WatchdogRec)));
        } else {
            SaHpiWatchdogT watchdog;
            SaErrorT error = saHpiWatchdogTimerGet(sessionId, rptEntry->ResourceId, 
                                                   rdr->RdrTypeUnion.WatchdogRec.WatchdogNum,
                                                   &watchdog);
            if (error != SA_OK) {
                status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
            } else if (watchdog.Running) {
                status.assertNotSupport();
            } else {
                status.add(TRACE, runWatchdogTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.WatchdogRec)));
            }
        }

        popLocation();
    }

    return status;
}


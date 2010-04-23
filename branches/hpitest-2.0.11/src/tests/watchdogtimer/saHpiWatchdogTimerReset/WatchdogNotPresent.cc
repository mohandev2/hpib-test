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

#include "WatchdogNotPresent.h"

using namespace ns_saHpiWatchdogTimerReset;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WatchdogNotPresent::WatchdogNotPresent(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *WatchdogNotPresent::getName() {
    return "WatchdogNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *WatchdogNotPresent::getDescription() {
    return "The Watchdog Timer identified by the <i>WatchdogNum</i> "
           "parameter does not exist.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *WatchdogNotPresent::getPrecondition() {
    return "Requires a resource that supports watchdogs.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT WatchdogNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus WatchdogNotPresent::runResourceTest(SaHpiSessionIdT sessionId,
                                                  SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasWatchdogCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiWatchdogTimerReset(sessionId,
                                                 rptEntry->ResourceId,
                                                 getInvalidWatchdogNum());
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, WATCHDOG_TIMER_RESET,
                                 SA_ERR_HPI_NOT_PRESENT, error,
                                 "Invalid WatchdogNum is 0x%X.",
                                 getInvalidWatchdogNum());
        }
    }

    return status;
}

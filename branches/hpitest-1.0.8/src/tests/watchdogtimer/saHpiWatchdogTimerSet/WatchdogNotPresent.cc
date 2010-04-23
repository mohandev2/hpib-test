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
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerSet;

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
    return "The Watchdog Timer identified by the <i>WatchdogNum</i>\n"
           "parameter is not present.";
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
    SaHpiWatchdogT watchdog;

    if (!hasWatchdogCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        WatchdogHelper::fill(&watchdog);
        SaErrorT error = saHpiWatchdogTimerSet(sessionId,
                                               rptEntry->ResourceId, 
                                               getInvalidWatchdogNum(),
                                               &watchdog);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET,
                                 SA_ERR_HPI_NOT_PRESENT, error);
        }
    }

    return status;
}

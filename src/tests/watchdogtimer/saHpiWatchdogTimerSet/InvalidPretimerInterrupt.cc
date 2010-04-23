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

#include "InvalidPretimerInterrupt.h"
#include "WatchdogHelper.h"

using namespace ns_saHpiWatchdogTimerSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidPretimerInterrupt::InvalidPretimerInterrupt(char *line)
: WatchdogTestCase(line, false) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidPretimerInterrupt::getName() {
    return "InvalidPretimerInterrupt";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidPretimerInterrupt::getDescription() {
    return "The <i>Watchdog->PretimerInterrupt</i> is not one of the\n"
           "valid enumerated values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidPretimerInterrupt::getPrecondition() {
    return "Requires one or more watchdog timers that are not running.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidPretimerInterrupt::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 * 
 * Test several invalid Pretimer Interrupts.
 *****************************************************************************/

HpiTestStatus InvalidPretimerInterrupt::runWatchdogTest(SaHpiSessionIdT sessionId,
                                                        SaHpiRptEntryT *rptEntry,
                                                        SaHpiWatchdogRecT *watchdogRec) {
    HpiTestStatus status;
    SaHpiWatchdogT watchdog;
    int invalidPretimerInterrupt[] = { (int) SAHPI_WPI_MESSAGE_INTERRUPT + 1,
                                       (int) SAHPI_WPI_OEM - 1,
                                       (int) SAHPI_WPI_OEM + 1 };

    WatchdogHelper::fill(&watchdog);
    for (int i = 0; i < 3; i++) {
        watchdog.PretimerInterrupt = (SaHpiWatchdogPretimerInterruptT) invalidPretimerInterrupt[i];
        SaErrorT error = saHpiWatchdogTimerSet(sessionId, rptEntry->ResourceId, 
                                               watchdogRec->WatchdogNum, &watchdog);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, WATCHDOG_TIMER_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Invalid PretimerInterrupt: %d",
                                 invalidPretimerInterrupt[i]);
        } 
    }

    return status;
}

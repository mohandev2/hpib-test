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
 * Changes
 *     Lars Wetzel     <Lars.Wetzel@emerson.com>
 *     AddWatchdog Callback
 */

#include "WatchdogHelper.h"
#include "Report.h"

/*****************************************************************************
 * Is the Watchdog Record valid or not?  NOTE: It is always valid.
 *****************************************************************************/

bool WatchdogHelper::isValid(SaHpiWatchdogRecT *, Report &) {
    return true;
}

/*****************************************************************************
 * Are the two Watchdog Records the same or not?  Include a report of
 * any differences.
 *****************************************************************************/

bool WatchdogHelper::isEqual(SaHpiWatchdogRecT *rec1,
                             SaHpiWatchdogRecT *rec2, Report &report) {
    bool equal = true;

    if (rec1->WatchdogNum != rec2->WatchdogNum) {
        equal = false;
        report.add("Watchdog Num fields are not equal [0x%X, 0x%X].",
                   rec1->WatchdogNum, rec2->WatchdogNum);
    }
    if (rec1->Oem != rec2->Oem) {
        equal = false;
        report.add("Watchdog->Oem fields are not equal [0x%X, 0x%X].",
                   rec1->Oem, rec2->Oem);
    }

    return equal;
}

/*****************************************************************************
 * Fill in a Watchdog with default values.
 *****************************************************************************/

void WatchdogHelper::fill(SaHpiWatchdogT *watchdog) {
    watchdog->Log                = SAHPI_FALSE;
    watchdog->Running            = SAHPI_FALSE;
    watchdog->TimerUse           = SAHPI_WTU_NONE;
    watchdog->TimerAction        = SAHPI_WA_NO_ACTION;
    watchdog->PretimerInterrupt  = SAHPI_WPI_NONE;
    watchdog->PreTimeoutInterval = 0;
    watchdog->TimerUseExpFlags   = 0;
    watchdog->InitialCount       = 0;
    watchdog->PresentCount       = 0;
}

/*****************************************************************************
 * Fill in a Watchdog with SMS default values.
 *****************************************************************************/

void WatchdogHelper::fillSms(SaHpiWatchdogT *watchdog) {
    watchdog->Log                = SAHPI_FALSE;
    watchdog->Running            = SAHPI_TRUE;
    watchdog->TimerUse           = SAHPI_WTU_SMS_OS;
    watchdog->TimerAction        = SAHPI_WA_RESET;
    watchdog->PretimerInterrupt  = SAHPI_WPI_NONE;
    watchdog->PreTimeoutInterval = 0;
    watchdog->TimerUseExpFlags   = SAHPI_WATCHDOG_EXP_SMS_OS;
    watchdog->InitialCount       = 900000; // 900 seconds
    watchdog->PresentCount       = 0;
}

/*****************************************************************************
 * Is this a valid watchdog?  Include a report with an errors.
 *****************************************************************************/

bool WatchdogHelper::isValid(SaHpiWatchdogT *watchdog, Report &report) {
    bool valid = true;

    if (!isValidWatchdogAction(watchdog->TimerAction)) {
        valid = false;
        report.add("Watchdog.TimerAction is invalid: %d.", watchdog->TimerAction);
    }

    if (!isValidWatchdogPretimerInterrupt(watchdog->PretimerInterrupt)) {
        valid = false;
        report.add("Watchdog.PretimerInterrupt is invalid: %d.", watchdog->PretimerInterrupt);
    }

    if (!isValidWatchdogTimerUse(watchdog->TimerUse)) {
        valid = false;
        report.add("Watchdog.TimerUse is invalid: %d.", watchdog->TimerUse);
    }

    return valid;
}

/*****************************************************************************
 * Is this a valid watchdog action?
 *****************************************************************************/

bool WatchdogHelper::isValidWatchdogAction(SaHpiWatchdogActionT action) {
    return (action == SAHPI_WA_NO_ACTION) ||
           (action == SAHPI_WA_RESET) ||
           (action == SAHPI_WA_POWER_DOWN) ||
           (action == SAHPI_WA_POWER_CYCLE);
}

/*****************************************************************************
 * Is this a valid watchdog pretimer interrupt?
 *****************************************************************************/

bool WatchdogHelper::isValidWatchdogPretimerInterrupt(SaHpiWatchdogPretimerInterruptT pretimer) {
    return (pretimer == SAHPI_WPI_NONE) ||
           (pretimer == SAHPI_WPI_SMI) ||
           (pretimer == SAHPI_WPI_NMI) ||
           (pretimer == SAHPI_WPI_MESSAGE_INTERRUPT) ||
           (pretimer == SAHPI_WPI_OEM);
}

/*****************************************************************************
 * Is this a valid watchdog timer use?
 *****************************************************************************/

bool WatchdogHelper::isValidWatchdogTimerUse(SaHpiWatchdogTimerUseT timerUse) {
    return (timerUse == SAHPI_WTU_NONE) ||
           (timerUse == SAHPI_WTU_BIOS_FRB2) ||
           (timerUse == SAHPI_WTU_BIOS_POST) ||
           (timerUse == SAHPI_WTU_OS_LOAD) ||
           (timerUse == SAHPI_WTU_SMS_OS) ||
           (timerUse == SAHPI_WTU_OEM) ||
           (timerUse == SAHPI_WTU_UNSPECIFIED);
}

/*****************************************************************************
 * For the TimerUse, get the Expiration flag.
 *****************************************************************************/

SaHpiWatchdogExpFlagsT WatchdogHelper::getExpirationFlag(SaHpiWatchdogTimerUseT timerUse) {
    switch (timerUse) {
        case SAHPI_WTU_BIOS_FRB2 : return SAHPI_WATCHDOG_EXP_BIOS_FRB2;
        case SAHPI_WTU_BIOS_POST : return SAHPI_WATCHDOG_EXP_BIOS_POST;
        case SAHPI_WTU_OS_LOAD : return SAHPI_WATCHDOG_EXP_OS_LOAD;
        case SAHPI_WTU_SMS_OS : return SAHPI_WATCHDOG_EXP_SMS_OS;
        case SAHPI_WTU_OEM : return SAHPI_WATCHDOG_EXP_OEM;
        default: return (SaHpiWatchdogExpFlagsT) 0;
    }
    return (SaHpiWatchdogExpFlagsT) 0;
}

/*****************************************************************************
 * Find Watchdogs to Test
 *****************************************************************************/

HpiTestStatus WatchdogHelper::findAllWatchdogs(SaHpiSessionIdT sessionId,
                                               WatchdogFinderCallback *cb) {
    HpiTestStatus status;
    SaHpiRptEntryT rptEntry;
    SaHpiRdrT rdr;
    SaHpiWatchdogT watchdog;

    SaHpiEntryIdT nextResourceEntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault() && nextResourceEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT resourceEntryId = nextResourceEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, resourceEntryId,
                                          &nextResourceEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else if (rptEntry.ResourceCapabilities & SAHPI_CAPABILITY_WATCHDOG) {
            SaHpiResourceIdT resourceId = rptEntry.ResourceId;
            SaHpiEntryIdT nextRdrEntryId = SAHPI_FIRST_ENTRY;
            while (!status.hasFault() && nextRdrEntryId != SAHPI_LAST_ENTRY) {
                SaHpiEntryIdT rdrEntryId = nextRdrEntryId;
                error = saHpiRdrGet(sessionId, resourceId,
                                    rdrEntryId, &nextRdrEntryId, &rdr);
                if (error == SA_ERR_HPI_NOT_PRESENT) {
                    break;
                } else if (error != SA_OK) {
                    status.assertError(TRACE, RDR_GET, SA_OK, error);
                    break;
                } else if ((rdr.RdrType == SAHPI_WATCHDOG_RDR)) {
                    SaHpiWatchdogRecT *watchdogRec = &(rdr.RdrTypeUnion.WatchdogRec);
                    error = saHpiWatchdogTimerGet(sessionId, resourceId,
                                                  watchdogRec->WatchdogNum, &watchdog);
                    if (error != SA_OK) {
                        status.assertError(TRACE, WATCHDOG_TIMER_GET, SA_OK, error);
                    } else {
                        cb->addWatchdog(sessionId, resourceId, watchdogRec->WatchdogNum, watchdog);
                    }
                }
            }
        }
    }

    return status;
}


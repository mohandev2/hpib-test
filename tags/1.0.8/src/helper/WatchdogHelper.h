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
 *
 * Changes:
 *     Lars Wetzel     <Lars.Wetzel@emerson.com>
 *     enhance addWatchdog with parameter sessionId
 *     
 */

#ifndef __WATCHDOGHELPER_H__
#define __WATCHDOGHELPER_H__

#include "SafTest.h"
#include "HpiTestStatus.h"

class Report;

/*****************************************************************************
 * Find Watchdog 
 *****************************************************************************/

class WatchdogFinderCallback
{
public:
    virtual ~WatchdogFinderCallback() {};
    virtual void addWatchdog(SaHpiSessionIdT sessionId,
			     SaHpiResourceIdT resourceId, 
                             SaHpiWatchdogNumT watchdogNum, 
                             SaHpiWatchdogT &watchdog) = 0;
};

/*****************************************************************************
 * Watchdog Helper
 *****************************************************************************/

class WatchdogHelper
{
public:
    static bool isValid(SaHpiWatchdogRecT *rec, Report &report);
    static bool isEqual(SaHpiWatchdogRecT *rec1, SaHpiWatchdogRecT *rec2, Report &report);

    static void fill(SaHpiWatchdogT *watchdog);
    static void fillSms(SaHpiWatchdogT *watchdog);

    static bool isValid(SaHpiWatchdogT *watchdog, Report &report);
    static bool isValidWatchdogAction(SaHpiWatchdogActionT action);
    static bool isValidWatchdogPretimerInterrupt(SaHpiWatchdogPretimerInterruptT pretimer);
    static bool isValidWatchdogTimerUse(SaHpiWatchdogTimerUseT timerUse);

    static SaHpiWatchdogExpFlagsT getExpirationFlag(SaHpiWatchdogTimerUseT timerUse);

    static HpiTestStatus findAllWatchdogs(SaHpiSessionIdT sessionId, 
                                          WatchdogFinderCallback *cb);
};

#endif


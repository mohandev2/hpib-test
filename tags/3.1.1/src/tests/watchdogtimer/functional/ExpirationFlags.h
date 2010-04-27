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

#ifndef __EXPIRATION_FLAGS_H__
#define __EXPIRATION_FLAGS_H__

#include "SafTest.h"
#include "HpiSessionTestCase.h"
#include "WatchdogHelper.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#ifndef MAX_WATCHDOGS
#define MAX_WATCHDOGS 100
#endif

/*****************************************************************************
 * Event Generation Test Case
 *****************************************************************************/

namespace ns_saHpiWatchdogFunctional
{
    class ExpirationFlags : public HpiSessionTestCase, public WatchdogFinderCallback
    {
    private:
        typedef struct {
            SaHpiResourceIdT resourceId;
            SaHpiWatchdogNumT watchdogNum;
            SaHpiWatchdogExpFlagsT timerUseExpFlags;
            SaHpiWatchdogT origWatchdog;
        } WatchdogData;

        int watchdogCount;
        WatchdogData watchdogData[MAX_WATCHDOGS];

    public:
        ExpirationFlags(char *line);

        HpiTestStatus runSessionTest(SaHpiSessionIdT sessionId);

        const char *getName();

        const char *getDescription();

        const char *getPrecondition();

        void addWatchdog(SaHpiSessionIdT sessionId,
			 SaHpiResourceIdT resourceId,
                         SaHpiWatchdogNumT watchdogNum,
                         SaHpiWatchdogT &watchdog);

    private:
        HpiTestStatus testWatchdogs(SaHpiSessionIdT sessionId);

        HpiTestStatus clearExpirationFlags(SaHpiSessionIdT sessionId);

        HpiTestStatus startWatchdogTimer(SaHpiSessionIdT sessionId,
                                         WatchdogData *watchdogData,
                                         SaHpiWatchdogTimerUseT timerUse,
                                         SaHpiUint32T initialCount);

        HpiTestStatus verifyExpirationFlags(SaHpiSessionIdT sessionId);

        HpiTestStatus clearExpirationFlagsOneByOne(SaHpiSessionIdT sessionId, 
                                                   WatchdogData *watchdogData);

        HpiTestStatus restoreWatchdogs(SaHpiSessionIdT sessionId);
    };
}

#endif

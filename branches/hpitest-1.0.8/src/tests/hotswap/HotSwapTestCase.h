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

#ifndef __HOTSWAPTESTCASE_H___
#define __HOTSWAPTESTCASE_H___

#include "SafTest.h"
#include "Time.h"
#include "HpiResourceTestCase.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define AUTO_TIMEOUT 2000000000 // 2 seconds

/*****************************************************************************
 * Base HotSwap Test Case
 *****************************************************************************/

class HotSwapTestCase : public HpiResourceTestCase
{
private:
    bool gatherInfo;
protected:
    bool hsAutoInsertReadOnly;
    bool hsAutoExtractReadOnly;
    SaHpiTimeoutT hsAutoInsertTimeout;
    SaHpiTimeoutT hsAutoExtractTimeout;
    SaHpiHsStateT hsState;
    bool hsIsHpiResource;

public:
    HotSwapTestCase();
    HotSwapTestCase(char* line);
    HotSwapTestCase(char* line, bool gatherInfo);

protected:
    int getTimeout();

    HpiTestStatus runResourceTest(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry);

    HpiTestStatus gatherHsInfo(SaHpiSessionIdT sessionId, SaHpiRptEntryT *rptEntry);

    virtual HpiTestStatus runHsTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry) = 0;

    bool isIndicatorSupported(SaHpiRptEntryT *rptEntry);

    HpiTestStatus restoreTimeouts(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId);

    HpiTestStatus setTimeouts(SaHpiSessionIdT sessionId,
                              SaHpiResourceIdT resourceId,
                              SaHpiTimeoutT timeout);

    HpiTestStatus extractResource(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId);

    HpiTestStatus insertResource(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId);

    HpiTestStatus waitForHsEvent(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId,
                                 SaHpiHsStateT state,
                                 TimeMsec timeout);

    HpiTestStatus getAutoInsertionTime(SaHpiSessionIdT sessionId,
                                       TimeMsec *time);

    HpiTestStatus getAutoExtractionTime(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        TimeMsec *time);

};

#endif


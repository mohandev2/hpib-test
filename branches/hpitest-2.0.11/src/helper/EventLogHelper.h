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
 *     Lars Wetzel     <lars.wetzel@emerson.com>
 *
 */

#ifndef __EVENTLOGHELPER_H__
#define __EVENTLOGHELPER_H__

#include "SafTest.h"
#include "HpiTestStatus.h"

#define FULL_EVT_CAPAPILITIES (SAHPI_EVTLOG_CAPABILITY_ENTRY_ADD | \
			       SAHPI_EVTLOG_CAPABILITY_CLEAR | \
			       SAHPI_EVTLOG_CAPABILITY_TIME_SET | \
			       SAHPI_EVTLOG_CAPABILITY_STATE_SET | \
			       SAHPI_EVTLOG_CAPABILITY_OVERFLOW_RESET)

/*****************************************************************************
 * EventLog Helper
 *****************************************************************************/

class EventLogHelper
{
public:
    static HpiTestStatus isEmpty(SaHpiSessionIdT sessionId, 
                                 SaHpiResourceIdT resourceId, bool *isEmpty);
    static HpiTestStatus addEntry(SaHpiSessionIdT sessionId, 
                                  SaHpiResourceIdT resourceId);
    static HpiTestStatus addEntry(SaHpiSessionIdT sessionId, 
                                  SaHpiResourceIdT resourceId, bool ignoreOutOfSpace);
    static HpiTestStatus addEntry(SaHpiSessionIdT sessionId, 
                                  SaHpiResourceIdT resourceId, SaHpiEventT *event);

    static HpiTestStatus fill(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId);
    static HpiTestStatus overflow(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId);

    static HpiTestStatus makeRoom(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId,
                                  int neededEntries);

    static HpiTestStatus findEvent(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId, 
                                   SaHpiEventT *event, bool *found);

    static bool         isValidCapability(SaHpiEventLogCapabilitiesT eventLogCapabilities);

    static bool         hasEvtLogAddCapability(SaHpiSessionIdT sessionId, 
					       SaHpiResourceIdT resourceId);
    
    static bool         hasEvtLogClearCapability(SaHpiSessionIdT sessionId, 
						 SaHpiResourceIdT resourceId);

    static bool         hasEvtLogTimerSetCapability(SaHpiSessionIdT sessionId, 
						    SaHpiResourceIdT resourceId);

    static bool         hasEvtLogStateSetCapability(SaHpiSessionIdT sessionId, 
						    SaHpiResourceIdT resourceId);

    static bool         hasEvtLogOverflowResetCapability(SaHpiSessionIdT sessionId, 
							 SaHpiResourceIdT resourceId);

    static bool         isSupportedEvtLogCapability(SaHpiSessionIdT sessionId, 
						    SaHpiResourceIdT resourceId,
						    SaHpiEventLogCapabilitiesT eventLogCapabilities);

    
};

#endif


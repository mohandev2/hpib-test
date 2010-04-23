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

#ifndef __EVENTHELPER_H__
#define __EVENTHELPER_H__

#include "SafTest.h"
#include "HpiTestStatus.h"
#include "EventMatcher.h"
#include "EventHandler.h"
#include "Time.h"

class Report;

/*****************************************************************************
 * Event Helper
 *****************************************************************************/

class EventHelper
{
public:
    static bool isEqual(SaHpiEventT *event1, SaHpiEventT *event2);
    static bool isEqual(SaHpiEventT *event1, SaHpiEventT *event2, Report &report);

    static void fill(SaHpiEventT *event);
    static void fill(SaHpiEventT *event, SaHpiSeverityT severity);
    static void fill(SaHpiEventT *event, SaHpiUint32T length);
    static void fill(SaHpiEventT *event, const char *text);
    static void fill(SaHpiEventT *event, SaHpiTextBufferT *buf);

    static HpiTestStatus clearQueue(SaHpiSessionIdT sessionId);

    static HpiTestStatus waitForEvent(SaHpiSessionIdT sessionId, 
                                      SaHpiEventT *eventToFind,
                                      SaHpiRdrT *rdr, 
                                      SaHpiRptEntryT *rptEntry, 
                                      SaHpiEvtQueueStatusT *eventQueueStatus,
                                      bool *foundEvent);

    static HpiTestStatus waitForEvent(SaHpiSessionIdT sessionId, EventMatcher &matcher,
                                      SaHpiEventT *event, SaHpiRdrT *rdr, SaHpiRptEntryT *rptEntry, 
                                      SaHpiEvtQueueStatusT *eventQueueStatus, bool *foundEvent);

    static HpiTestStatus waitForEvent(SaHpiSessionIdT sessionId, EventMatcher &matcher, TimeMsec timeout,
                                      SaHpiEventT *event, SaHpiRdrT *rdr, SaHpiRptEntryT *rptEntry, 
                                      SaHpiEvtQueueStatusT *eventQueueStatus, bool *foundEvent);

    static HpiTestStatus pollEvents(SaHpiSessionIdT sessionId, long timeoutSeconds,
                                    EventMatcher &matcher, EventHandler *handler);

    static HpiTestStatus generateSensorChangeEvent(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry,
                                                   SaHpiRdrT *rdr,
                                                   EventMatcher *matcher);

private:
    static HpiTestStatus findSensorForChange(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiRdrT *rdr,
                                             int *change);

    static HpiTestStatus toggleSensorEnable(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId,
                                            SaHpiSensorNumT sensorNum,
                                            EventMatcher *matcher);

    static HpiTestStatus toggleSensorEventEnable(SaHpiSessionIdT sessionId,
                                                 SaHpiResourceIdT resourceId,
                                                 SaHpiSensorNumT sensorNum,
                                                 EventMatcher *matcher);
};

#endif

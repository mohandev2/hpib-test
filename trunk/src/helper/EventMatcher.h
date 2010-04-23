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
 * 09/06/24 lars.wetzel@emerson.com
 *          Add Matching functionality for ResourceEventType
 */

#ifndef __EVENTMATCHER_H__
#define __EVENTMATCHER_H__

#include "SafTest.h"

/*****************************************************************************
 * Event Matcher
 *
 * The Event Matcher is used to match events that show up in the Event Queue.
 * The "set" methods are used to determine what events to match against.  The
 * matches() method determines if an event does match.  This is extremely
 * useful.  In many cases, an HPI Test case is looking for a specific event
 * and must therefore skip over unrelated events in the Event Queue. 
 *****************************************************************************/

class EventMatcher
{
private:
    bool matchSource;
    SaHpiResourceIdT source;

    bool matchEventType;
    SaHpiEventTypeT eventType;

    bool matchSeverity;
    SaHpiSeverityT severity;

    bool matchResourceEventType;
    int eventStateCount;
    SaHpiResourceEventTypeT resEvtType[5];

    bool matchHotSwapState;
    int hotSwapStateCount;
    SaHpiHsStateT hotSwapState[5];

    bool matchSensorNum;
    SaHpiSensorNumT sensorNum;

    bool matchSensorEventState;
    SaHpiEventStateT sensorEventState;

    bool matchSensorAssertion;
    SaHpiBoolT sensorAssertion;

public:
    EventMatcher();

    void setSource(SaHpiResourceIdT resourceId);
    void setEventType(SaHpiEventTypeT eventType);
    void setSeverity(SaHpiSeverityT severity);

    void setResourceEventType(SaHpiResourceEventTypeT resourceEvtType);
    void addResourceEventType(SaHpiResourceEventTypeT resourceEvtType);

    void setHotSwapState(SaHpiHsStateT hotSwapState);
    void addHotSwapState(SaHpiHsStateT hotSwapState);

    void setSensorNum(SaHpiSensorNumT sensorNum);
    void setSensorAssertion(SaHpiBoolT sensorAssertion);
    void setSensorEventState(SaHpiEventStateT sensorEventState);

    bool matches(SaHpiEventT &event);
};

#endif

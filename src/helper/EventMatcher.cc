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
 * 09/10/30 larswetzel@users.sourceforge.net
 *          Enh Resource and HotSwap matching functionality to allow both types
 * 09/10/14 larswetzel@users.sourceforge.net
 *          Enh ResourceEvent matching functionality
 * 09/06/24 lars.wetzel@emerson.com
 *          Add Matching functionality for ResourceEventType
 *
 */

#include "EventMatcher.h"
#include "HpiHelper.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EventMatcher::EventMatcher() {
    matchSource = false;
    matchEventType = false;
    matchSeverity = false;
    matchHotSwapState = false;
    hotSwapStateCount = 0;
    eventStateCount = 0;
    matchSensorNum = false;
    matchSensorAssertion = false;
    matchSensorEventState = false;
    matchResourceEventType = false;
}

/*****************************************************************************
 * Set the source of the event that we are looking for.
 *****************************************************************************/

void EventMatcher::setSource(SaHpiResourceIdT resourceId) {
    this->matchSource = true;
    this->source = resourceId;
}

/*****************************************************************************
 * Set the type of event we are looking for.
 *****************************************************************************/

void EventMatcher::setEventType(SaHpiEventTypeT evtType) {
    this->matchEventType = true;
    this->eventType = evtType;
}

/*****************************************************************************
 * Set the severity of the event we are looking for.
 *****************************************************************************/

void EventMatcher::setSeverity(SaHpiSeverityT s) {
    this->matchSeverity = true;
    this->severity = s;
}

/*****************************************************************************
 * Set the resource event type of the event we are looking for.
 *****************************************************************************/

void EventMatcher::setResourceEventType(SaHpiResourceEventTypeT resourceEvtType) {
    this->matchResourceEventType = true;
    this->eventStateCount = 1;
    this->resEvtType[0] = resourceEvtType;
}

/*****************************************************************************
 * Add another resource event type that we are looking for.
 *****************************************************************************/

void EventMatcher::addResourceEventType(SaHpiResourceEventTypeT resourceEvtType) {
    this->matchResourceEventType = true;
    this->resEvtType[this->eventStateCount] = resourceEvtType;
    this->eventStateCount++;
}
/*****************************************************************************
 * Set the hot swap state of the event we are looking for.
 *****************************************************************************/

void EventMatcher::setHotSwapState(SaHpiHsStateT hsState) {
    this->matchHotSwapState = true;
    this->hotSwapStateCount = 1;
    this->hotSwapState[0] = hsState;
}

/*****************************************************************************
 * Add another hot swap state that we are looking for.
 *****************************************************************************/

void EventMatcher::addHotSwapState(SaHpiHsStateT hsState) {
    this->matchHotSwapState = true;
    this->hotSwapState[this->hotSwapStateCount] = hsState;
    this->hotSwapStateCount++;
}

/*****************************************************************************
 * Set the SensorNum for a SensorEvent or SensorEnableChangeEvent 
 * we are looking for.
 *****************************************************************************/

void EventMatcher::setSensorNum(SaHpiSensorNumT num) {
    this->matchSensorNum = true;
    this->sensorNum = num;
}

/*****************************************************************************
 * Set the Assertion for a SensorEvent we are looking for.
 *****************************************************************************/

void EventMatcher::setSensorAssertion(SaHpiBoolT assertion) {
    this->matchSensorAssertion = true;
    this->sensorAssertion = assertion;
}

/*****************************************************************************
 * Set the EventState for a SensorEvent we are looking for.
 *****************************************************************************/

void EventMatcher::setSensorEventState(SaHpiEventStateT eventState) {
    this->matchSensorEventState = true;
    this->sensorEventState = eventState;
}

/*****************************************************************************
 * Does this event match the type of event we are looking for?
 *****************************************************************************/

bool EventMatcher::matches(SaHpiEventT &event) {
    if (matchSource) {
        if (event.Source != source) {
            return false;
        }
    }

    if (matchEventType) {
        if (event.EventType != eventType) {
            return false;
        }
    }

    if (matchSeverity) {
        if (event.Severity != severity) {
            return false;
        }
    }

    if (matchHotSwapState) {
        if (event.EventType == SAHPI_ET_HOTSWAP) {
        	    // does the hotswap state match any of the given hot swap states?
            bool match = false;
            for (int i = 0; i < hotSwapStateCount && !match; i++) {
            	    if (event.EventDataUnion.HotSwapEvent.HotSwapState == hotSwapState[i]) {
            	    	    match = true;
            	    	}
            }
            if (!match) {
            	    return false;
            }
        }
    }

    if (matchResourceEventType) {
    	    if (event.EventType == SAHPI_ET_RESOURCE) {
    	        // does the ResourceEvent match any of the given types?
    	        bool match = false;
    	        for (int i = 0; i < eventStateCount && !match; i++) {
                if (event.EventDataUnion.ResourceEvent.ResourceEventType == resEvtType[i]) {
                	    match = true;
                }
            }
            if (!match) {
                return false;
            }
    	    }    
    }

    if (event.EventType == SAHPI_ET_SENSOR_ENABLE_CHANGE) {
        if (matchSensorNum) {
            if (event.EventDataUnion.SensorEnableChangeEvent.SensorNum != sensorNum) {
                return false;
            }
        }
    } else if (event.EventType == SAHPI_ET_SENSOR) {
        SaHpiSensorEventT *sensorEvent = &event.EventDataUnion.SensorEvent;
        if (matchSensorNum) {
            if (sensorEvent->SensorNum != sensorNum) {
                return false;
            }
        }

        if (matchSensorAssertion) {
            if (!HpiHelper::isBoolEqual(sensorEvent->Assertion, sensorAssertion)) {
                return false;
            }
        }

        if (matchSensorEventState) {
            if (sensorEvent->EventState != sensorEventState) {
                return false;
            }
        }
    }

    return true;
}

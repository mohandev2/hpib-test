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
 */

#include "EventLogHelper.h"
#include "EventHelper.h"
#include "TextBufferHelper.h"

/*****************************************************************************
 * Is the EventLog empty?
 *****************************************************************************/

HpiTestStatus EventLogHelper::isEmpty(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId, bool *isEmpty) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else {
        *isEmpty = (info.Entries == 0);
    }

    return status;
}

/*****************************************************************************
 * Add an eventlog entry to the EventLog.  
 *****************************************************************************/

HpiTestStatus EventLogHelper::addEntry(SaHpiSessionIdT sessionId, 
                                       SaHpiResourceIdT resourceId,
                                       bool ignoreOutOfSpace) {
    HpiTestStatus status;
    SaHpiEventT event;

    EventHelper::fill(&event);
    SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (ignoreOutOfSpace && error == SA_ERR_HPI_OUT_OF_SPACE) {
        // do nothing
    } else if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_ENTRY_ADD, SA_OK, error);
    }

    return status;
}

/*****************************************************************************
 * Add an eventlog entry to the EventLog.
 *****************************************************************************/

HpiTestStatus EventLogHelper::addEntry(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId) {
    return addEntry(sessionId, resourceId, false);
}

/*****************************************************************************
 * Add the given event to the EventLog.
 *****************************************************************************/

HpiTestStatus EventLogHelper::addEntry(SaHpiSessionIdT sessionId, 
                                       SaHpiResourceIdT resourceId,
                                       SaHpiEventT *event) {
    HpiTestStatus status;

    SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, event);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_ENTRY_ADD, SA_OK, error);
    }

    return status;
}

/*****************************************************************************
 * Fill the EventLog, but do not overflow it.  This method assumes that the
 * EventLog has been disabled so that no system eventlog entries will 
 * unexpectedly appear.
 *****************************************************************************/

HpiTestStatus EventLogHelper::fill(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.Size == 0) {
        status.assertNotSupport();
    } else {
        SaHpiUint32T count = info.Size - info.Entries;
        for (SaHpiUint32T i = 0; i < count; i++) {
            status = addEntry(sessionId, resourceId, true);
            if (!status.isOkay()) {
                break;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Make room in the EventLog to add the given number of entries.  Since
 * system event log entries can get added to the Event Log at any time,
 * it is necessary to give some extra room as a buffer.  In this case, we
 * will use 100 extra entries.  If we don't have enough room, then we will
 * clear the event log.
 *****************************************************************************/

HpiTestStatus EventLogHelper::makeRoom(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId,
                                       int neededEntries) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.Size == 0) {
        status.assertNotSupport();
    } else if (info.OverflowAction == SAHPI_EL_OVERFLOW_DROP) {
        if (info.Entries + neededEntries + 100 > info.Size) {
            error = saHpiEventLogClear(sessionId, resourceId);
            status.checkError(TRACE, EVENT_LOG_CLEAR, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Overflow the EventLog.  This method assumes that the EventLog has been 
 * disabled so that no system eventlog entries will unexpectedly appear.
 *****************************************************************************/

HpiTestStatus EventLogHelper::overflow(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.Size == 0) {
        status.assertNotSupport();
    } else {
        SaHpiUint32T count = info.Size - info.Entries + 1;
        for (SaHpiUint32T i = 0; i < count; i++) {
            status = addEntry(sessionId, resourceId, true);
            if (!status.isOkay()) {
                break;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Search the EventLog looking for an entry with the same given event.
 *****************************************************************************/

HpiTestStatus EventLogHelper::findEvent(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiEventT *event, bool *found) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT entryId;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    *found = false;
    prevEntryId = SAHPI_NEWEST_ENTRY;
    while (prevEntryId != SAHPI_NO_MORE_ENTRIES) {
        entryId = prevEntryId;
        SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId, entryId, 
                                               &prevEntryId, &nextEntryId, &eventLogEntry, 
                                               NULL, NULL);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_ENTRY_GET, SA_OK, error);
            break;
        } else if (EventHelper::isEqual(event, &eventLogEntry.Event)) {
            *found = true;
            break;
        } 
    }

    return status;
}

/*****************************************************************************
 * Check the EventLog Capabilities
 *****************************************************************************/
bool EventLogHelper::isSupportedEvtLogCapability(SaHpiSessionIdT sessionId, 
						SaHpiResourceIdT resourceId,
						SaHpiEventLogCapabilitiesT eventLogCapabilities) {

  SaHpiEventLogCapabilitiesT evtLogCap;
  HpiTestStatus status;

  SaErrorT error = saHpiEventLogCapabilitiesGet(sessionId, resourceId, &evtLogCap);
  if (error != SA_OK) {
    status.assertError(TRACE, EVENT_LOG_CAPABILITIES_GET, SA_OK, error);
  } 
  
  return (eventLogCapabilities & evtLogCap);
}


bool  EventLogHelper::isValidCapability(SaHpiEventLogCapabilitiesT capabilities) {
  return (capabilities <= FULL_EVT_CAPAPILITIES);
}

bool  EventLogHelper::hasEvtLogAddCapability(SaHpiSessionIdT sessionId, 
					     SaHpiResourceIdT resourceId) {
  return isSupportedEvtLogCapability(sessionId, resourceId, SAHPI_EVTLOG_CAPABILITY_ENTRY_ADD);
}
    
bool EventLogHelper::hasEvtLogClearCapability(SaHpiSessionIdT sessionId, 
					     SaHpiResourceIdT resourceId) {
  return isSupportedEvtLogCapability(sessionId, resourceId, SAHPI_EVTLOG_CAPABILITY_CLEAR);
}

bool EventLogHelper::hasEvtLogTimerSetCapability(SaHpiSessionIdT sessionId, 
						SaHpiResourceIdT resourceId){
  return isSupportedEvtLogCapability(sessionId, resourceId, SAHPI_EVTLOG_CAPABILITY_TIME_SET);
}

bool EventLogHelper::hasEvtLogStateSetCapability(SaHpiSessionIdT sessionId, 
						SaHpiResourceIdT resourceId){
  return isSupportedEvtLogCapability(sessionId, resourceId, SAHPI_EVTLOG_CAPABILITY_STATE_SET);
}

bool EventLogHelper::hasEvtLogOverflowResetCapability(SaHpiSessionIdT sessionId, 
						     SaHpiResourceIdT resourceId){
  return isSupportedEvtLogCapability(sessionId, resourceId, SAHPI_EVTLOG_CAPABILITY_OVERFLOW_RESET);
}


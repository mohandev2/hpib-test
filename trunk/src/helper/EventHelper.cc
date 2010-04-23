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

#include "EventHelper.h"
#include "TextBufferHelper.h"
#include "Time.h"
#include "Timer.h"
#include "Report.h"
#include "HpiString.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define TIMEOUT 10000 // milliseconds (10 seconds)
#define MSEC_TO_NANOSEC 1000000

#define CHANGE_NONE                0
#define CHANGE_SENSOR_ENABLE       1
#define CHANGE_SENSOR_EVENT_ENABLE 2

/*****************************************************************************
 * Are the two events the same or not?
 *****************************************************************************/

bool EventHelper::isEqual(SaHpiEventT *event1, SaHpiEventT *event2) {
    Report report;
    return isEqual(event1, event2, report);
}

/*****************************************************************************
 * Are the two events the same or not?
 *****************************************************************************/

bool EventHelper::isEqual(SaHpiEventT *event1, SaHpiEventT *event2, Report &report) {
    bool equal = true;

    if (event1->Source != event2->Source) {
        equal = false;
        report.add("Event->Source fields are not equal [0x%X, 0x%X].",
                   event1->Source, event2->Source);
    } 
    
    if (event1->EventType != event2->EventType) {
        equal = false;
        report.add("Event->EventType fields are not equal [%s, %s].",
                   HpiString::eventType(event1->EventType),
                   HpiString::eventType(event2->EventType));
    } 
    
    if (event1->Severity != event2->Severity) {
        equal = false;
        report.add("Event->Severity fields are not equal [%s, %s].",
                   HpiString::severity(event1->Severity),
                   HpiString::severity(event2->Severity));
    } 

    // If an added event has an unspecified timestamp, then the
    // implementation can set it; so, whenever we compare two
    // events, if one or both has an unspecified timestamp, don't
    // bother doing the comparison.
    
    if ((event1->Timestamp != SAHPI_TIME_UNSPECIFIED) && 
        (event2->Timestamp != SAHPI_TIME_UNSPECIFIED)) {

        if (event1->Timestamp != event2->Timestamp) {
            equal = false;
            report.add("Event->Timestamp fields are not equal [%lld, %lld].",
                       event1->Timestamp, event2->Timestamp);
        } 
    }
    
    if (event1->EventType == SAHPI_ET_USER && event2->EventType == SAHPI_ET_USER) {
        if (!TextBufferHelper::isEqual(&event1->EventDataUnion.UserEvent.UserEventData,
                                       &event2->EventDataUnion.UserEvent.UserEventData,
                                       report)) {
            equal = false;
        }
    }

    return equal;
}

/*****************************************************************************
 * Fill the event with user data.  Useful for adding a user event to the 
 * HPI system.
 *****************************************************************************/

void EventHelper::fill(SaHpiEventT *event) {
    fill(event, SAHPI_OK);
}

/*****************************************************************************
 * Fill the event with user data and a given severity.  Useful for adding a 
 * user event to the HPI system.
 *****************************************************************************/

void EventHelper::fill(SaHpiEventT *event, SaHpiSeverityT severity) {
    event->Source = SAHPI_UNSPECIFIED_RESOURCE_ID;
    event->EventType = SAHPI_ET_USER;
    event->Timestamp = SAHPI_TIME_UNSPECIFIED;
    event->Severity = severity;
    TextBufferHelper::fill(&event->EventDataUnion.UserEvent.UserEventData);
}

/*****************************************************************************
 * Fill the event with user data and with a specific number of bytes in
 * the text buffer.  Useful for adding a user event to the HPI system.
 *****************************************************************************/

void EventHelper::fill(SaHpiEventT *event, SaHpiUint32T dataLength) {
    event->EventType = SAHPI_ET_USER;
    event->Source = SAHPI_UNSPECIFIED_RESOURCE_ID;
    event->Timestamp = SAHPI_TIME_UNSPECIFIED;
    event->Severity = SAHPI_OK;
    TextBufferHelper::fill(&event->EventDataUnion.UserEvent.UserEventData, dataLength);
}

/*****************************************************************************
 * Fill the event with user data and with a specific text string to put
 * into the text buffer.  Useful for adding a user event to the HPI system.
 *****************************************************************************/

void EventHelper::fill(SaHpiEventT *event, const char *text) {
    event->EventType = SAHPI_ET_USER;
    event->Source = SAHPI_UNSPECIFIED_RESOURCE_ID;
    event->Timestamp = SAHPI_TIME_UNSPECIFIED;
    event->Severity = SAHPI_OK;
    TextBufferHelper::fill(&event->EventDataUnion.UserEvent.UserEventData, text);
}

/*****************************************************************************
 * Fill the event with user data and with a specific text buffer.
 * Useful for adding a user event to the HPI system.
 *****************************************************************************/

void EventHelper::fill(SaHpiEventT *event, SaHpiTextBufferT *buffer) {
    event->EventType = SAHPI_ET_USER;
    event->Source = SAHPI_UNSPECIFIED_RESOURCE_ID;
    event->Timestamp = SAHPI_TIME_UNSPECIFIED;
    event->Severity = SAHPI_OK;
    event->EventDataUnion.UserEvent.UserEventData = *buffer;
}

/*****************************************************************************
 * Clear the Event Queue.  Keep getting events until there are no more.
 * Of course, more could appear immediately after we exit this loop, but
 * we can't do anything about that in this method.
 *****************************************************************************/

HpiTestStatus EventHelper::clearQueue(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;

    while (true) {
        SaErrorT error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_IMMEDIATE,
                                       &event, NULL, NULL, NULL);
        if (error == SA_ERR_HPI_TIMEOUT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, EVENT_GET, SA_OK, error);
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * Wait for a period of time for the given event to occur.  Set found to
 * indicate whether the event was found or not.
 *****************************************************************************/

HpiTestStatus EventHelper::waitForEvent(SaHpiSessionIdT sessionId,
                                        SaHpiEventT *eventToFind,
                                        SaHpiRdrT *rdr,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiEvtQueueStatusT *eventQueueStatus,
                                        bool *foundEvent) {
    HpiTestStatus status;
    SaHpiEventT event;

    *foundEvent = false;
    TimeMsec duration = TIMEOUT;
    while (duration > 0) {
        TimeMsec startTime = Time::getCurrent();
        SaHpiTimeoutT timeoutNano = duration * MSEC_TO_NANOSEC;
        SaErrorT error = saHpiEventGet(sessionId, timeoutNano, &event,
                                       rdr, rptEntry, eventQueueStatus);
        if (error == SA_ERR_HPI_TIMEOUT) {
            //do nothing
        } else if (error != SA_OK) {
            status.assertError(TRACE, EVENT_GET, SA_OK, error);
            break;
        } else if (EventHelper::isEqual(eventToFind, &event)) {
            *foundEvent = true;
            break;
        }

        TimeMsec elapsedTime = Time::getCurrent() - startTime;
        duration -= elapsedTime;
    }

    return status;
}

/*****************************************************************************
 * Wait for a specific event to occur.  If an event occurs that matches
 * the EventMatcher, then the method will exit with that event.  If no
 * matching event is found, the method will exit after 5 seconds with 
 * "foundEvent" set to false.
 *****************************************************************************/

HpiTestStatus EventHelper::waitForEvent(SaHpiSessionIdT sessionId, EventMatcher &matcher,
                                        SaHpiEventT *event, SaHpiRdrT *rdr, SaHpiRptEntryT *rptEntry, 
                                        SaHpiEvtQueueStatusT *eventQueueStatus, bool *foundEvent) {
    return waitForEvent(sessionId, matcher, TIMEOUT, event, rdr, rptEntry, eventQueueStatus, foundEvent);
}

/*****************************************************************************
 * Wait for a specific event to occur.  If an event occurs that matches
 * the EventMatcher, then the method will exit with that event.  If no
 * matching event is found, the method will exit after timeout msec with 
 * "foundEvent" set to false.
 *****************************************************************************/

HpiTestStatus EventHelper::waitForEvent(SaHpiSessionIdT sessionId, EventMatcher &matcher, TimeMsec timeout,
                                        SaHpiEventT *event, SaHpiRdrT *rdr, SaHpiRptEntryT *rptEntry, 
                                        SaHpiEvtQueueStatusT *eventQueueStatus, bool *foundEvent) {
    HpiTestStatus status;

    *foundEvent = false;
    TimeMsec duration = timeout;
    while (duration > 0) {
        TimeMsec startTime = Time::getCurrent();
        SaHpiTimeoutT timeoutNano = duration * MSEC_TO_NANOSEC;
        SaErrorT error = saHpiEventGet(sessionId, timeoutNano, event, rdr, rptEntry, eventQueueStatus);
        if (error == SA_ERR_HPI_TIMEOUT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, EVENT_GET, SA_OK, SA_ERR_HPI_TIMEOUT, error);
            break;
        } else if (matcher.matches(*event)) {
            *foundEvent = true;
            break;
        }

        TimeMsec elapsedTime = Time::getCurrent() - startTime;
        duration -= elapsedTime;
    }

    return status;
}

/*****************************************************************************
 * Poll for events looking for events that match the EventMatcher.  When an
 * event is matched, the Event Handler is notified.  
 *
 * The argument "timeoutSeconds" determines how long this method will continue
 * to poll for events.  If set to -1, this method will continue as long as
 * there are events in the Event Queue.  If set to another value, this method
 * will wait that duration for other events to show up in the Event Queue.
 *
 * Lastly, it is possible for the Event Handler to determine that it is not
 * necessary to continue waiting for events.  If so, "isEventProcessingDone()"
 * returns true and this method will exit.  In other words, if the 
 * "timeoutSeconds" is 10 seconds, but after 5 seconds the Event Handler 
 * returns true, we will immediately stop polling for events.
 *****************************************************************************/

HpiTestStatus EventHelper::pollEvents(SaHpiSessionIdT sessionId, long timeoutSeconds,
                                      EventMatcher &matcher, EventHandler *handler) {
    HpiTestStatus status;
    SaHpiEventT event;

    if (timeoutSeconds == -1) {
        while (!handler->isEventProcessingDone()) {
            SaErrorT error = saHpiEventGet(sessionId, SAHPI_TIMEOUT_IMMEDIATE, &event, NULL, NULL, NULL);
            if (error == SA_ERR_HPI_TIMEOUT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, EVENT_GET, SA_OK, SA_ERR_HPI_TIMEOUT, error);
                break;
            } else if (matcher.matches(event)) {
                HpiTestStatus handlerStatus = handler->handleEvent(sessionId, &event);
                if (handlerStatus.hasFault()) {
                    status.add(TRACE, handlerStatus);
                    break;
                }
            }
        }
    } else {
        TimeMsec duration = timeoutSeconds * 1000;
        while (duration > 0 && !handler->isEventProcessingDone()) {
            TimeMsec startTime = Time::getCurrent();
            SaHpiTimeoutT timeoutNano = duration * MSEC_TO_NANOSEC;
            SaErrorT error = saHpiEventGet(sessionId, timeoutNano, &event, NULL, NULL, NULL);
            if (error == SA_ERR_HPI_TIMEOUT) {
                // do nothing
            } else if (error != SA_OK) {
                status.assertError(TRACE, EVENT_GET, SA_OK, SA_ERR_HPI_TIMEOUT, error);
                break;
            } else if (matcher.matches(event)) {
                HpiTestStatus handlerStatus = handler->handleEvent(sessionId, &event);
                if (handlerStatus.hasFault()) {
                    status.add(TRACE, handlerStatus);
                    break;
                }
            }

            TimeMsec elapsedTime = Time::getCurrent() - startTime;
            duration -= elapsedTime;
        }
    }

    return status;
}

/*****************************************************************************
 * Generate a Sensor Change Event.  Typically, this method is used to
 * generate a system event.
 *
 * Arguments:
 *     sessionId : the session id (input)
 *     rptEntry  : the resource's RPT entry (output)
 *     rdr       : the resource's RDR entry (output)
 *     matcher   : used to match against system event (output)
 *
 * If we can't find a sensor in domain for which we can generate an event,
 * the status will be set to Not Supported.
 *
 * NOTE: If a calling program needs to get the event from the event queue
 *       then it should do a wait for a period of time.
 *****************************************************************************/

HpiTestStatus EventHelper::generateSensorChangeEvent(SaHpiSessionIdT sessionId,
                                                     SaHpiRptEntryT *rptEntry,
                                                     SaHpiRdrT *rdr,
                                                     EventMatcher *matcher) {
    HpiTestStatus status;
    int change;
    SaHpiRptEntryT myRptEntry;
    SaHpiRdrT myRdr;

    HpiTestStatus findStatus = 
                    findSensorForChange(sessionId, &myRptEntry, &myRdr, &change);
    if (!findStatus.isOkay()) {
        status.add(TRACE, findStatus);
    } else if (change == CHANGE_NONE) {
        status.assertNotSupport();
    } else if (change == CHANGE_SENSOR_ENABLE) {
        status.add(TRACE, toggleSensorEnable(sessionId, myRptEntry.ResourceId,
                                             myRdr.RdrTypeUnion.SensorRec.Num,
                                             matcher));
    } else if (change == CHANGE_SENSOR_EVENT_ENABLE) {
        status.add(TRACE, toggleSensorEventEnable(sessionId, myRptEntry.ResourceId,
                                                  myRdr.RdrTypeUnion.SensorRec.Num,
                                                  matcher));
    }

    if (rptEntry != NULL) {
        *rptEntry = myRptEntry;
    }

    if (rdr != NULL) {
        *rdr = myRdr;
    }

    return status;
}

/*****************************************************************************
 * Find a sensor whose enable status or event enable status can be changed.
 *****************************************************************************/

HpiTestStatus EventHelper::findSensorForChange(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry,
                                               SaHpiRdrT *rdr,
                                               int *change) {
    HpiTestStatus status;

    *change = CHANGE_NONE;
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY && *change == CHANGE_NONE) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId, &nextEntryId, rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else if (rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_SENSOR) {
            SaHpiEntryIdT nextRdrEntryId = SAHPI_FIRST_ENTRY;
            while ((!status.hasFault()) && 
                   (nextRdrEntryId != SAHPI_LAST_ENTRY) && 
                   (*change == CHANGE_NONE)) {

                SaHpiEntryIdT rdrEntryId = nextRdrEntryId;
                error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                    rdrEntryId, &nextRdrEntryId, rdr);
                if (error == SA_ERR_HPI_NOT_PRESENT) {
                    break;
                } else if (error != SA_OK) {
                    status.assertError(TRACE, RDR_GET, SA_OK, error);
                    break;
                } else if (rdr->RdrType == SAHPI_SENSOR_RDR) {
                    SaHpiSensorRecT *sensorRec = &(rdr->RdrTypeUnion.SensorRec);
                    if (sensorRec->EnableCtrl) {
                        *change = CHANGE_SENSOR_ENABLE;
                    } else if (sensorRec->EventCtrl != SAHPI_SEC_READ_ONLY) {
                        *change = CHANGE_SENSOR_EVENT_ENABLE;
                    }
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Toggle the sensor's enable state in order to generate an event.
 *****************************************************************************/

HpiTestStatus EventHelper::toggleSensorEnable(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId,
                                              SaHpiSensorNumT sensorNum,
                                              EventMatcher *matcher) {
    HpiTestStatus status;
    SaHpiBoolT origEnabled;

    SaErrorT error = saHpiSensorEnableGet(sessionId, resourceId,
                                          sensorNum, &origEnabled);
    if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error,
                           "Resource: 0x%X, SensorNum: 0x%X",
                           resourceId, sensorNum);
    } else {
        SaHpiBoolT enabled = !origEnabled;
        error = saHpiSensorEnableSet(sessionId, resourceId, sensorNum, enabled);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_ENABLE_SET, SA_OK, error);
        } else {
            // pause to make sure the event gets generated
            Timer::sleep(2);

            error = saHpiSensorEnableSet(sessionId, resourceId, sensorNum, origEnabled);
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_ENABLE_SET, SA_OK, error);
            } else if (matcher) {
                matcher->setSource(resourceId);
                matcher->setEventType(SAHPI_ET_SENSOR_ENABLE_CHANGE);
                matcher->setSensorNum(sensorNum);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Toggle the sensor's event enable state in order to generate an event.
 *****************************************************************************/

HpiTestStatus EventHelper::toggleSensorEventEnable(SaHpiSessionIdT sessionId,
                                                   SaHpiResourceIdT resourceId,
                                                   SaHpiSensorNumT sensorNum,
                                                   EventMatcher *matcher) {
    HpiTestStatus status;
    SaHpiBoolT origEnabled;

    SaErrorT error = saHpiSensorEventEnableGet(sessionId, resourceId,
                                               sensorNum, &origEnabled);
    if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_ENABLE_GET, SA_OK, error,
                           "Resource: 0x%X, SensorNum: 0x%X",
                           resourceId, sensorNum);
    } else {
        SaHpiBoolT enabled = !origEnabled;
        error = saHpiSensorEventEnableSet(sessionId, resourceId, sensorNum, enabled);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_ENABLE_SET, SA_OK, error);
        } else {
            // pause to make sure the event gets generated
            Timer::sleep(2);

            error = saHpiSensorEventEnableSet(sessionId, resourceId, sensorNum, origEnabled);
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_EVENT_ENABLE_SET, SA_OK, error);
            } else if (matcher) {
                matcher->setSource(resourceId);
                matcher->setEventType(SAHPI_ET_SENSOR_ENABLE_CHANGE);
                matcher->setSensorNum(sensorNum);
            }
        }
    }

    return status;
}

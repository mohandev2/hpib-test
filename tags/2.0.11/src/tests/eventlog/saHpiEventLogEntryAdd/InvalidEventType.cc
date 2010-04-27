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

#include "InvalidEventType.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidEventType::InvalidEventType(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidEventType::getName() {
    return "InvalidEventType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidEventType::getDescription() {
    return "The event structure passed via the <i>EvtEntry</i> parameter is\n"
           "not an event of type SAHPI_ET_USER with the <i>Source</i> field set\n"
           "to SAHPI_UNSPECIFIED_RESOURCE_ID.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidEventType::getPrecondition() {
    return "Requires the ability to add an entry to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidEventType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidEventType::runAddTest(SaHpiSessionIdT sessionId,
                                           SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventTypeT invalidEventType[] = {
        SAHPI_ET_RESOURCE, SAHPI_ET_DOMAIN,
        SAHPI_ET_SENSOR, SAHPI_ET_SENSOR_ENABLE_CHANGE,
        SAHPI_ET_HOTSWAP, SAHPI_ET_WATCHDOG,
        SAHPI_ET_HPI_SW, SAHPI_ET_OEM };

    if (EventLogHelper::hasEvtLogAddCapability(sessionId, resourceId)) {
      EventHelper::fill(&event);
      for (int i = 0; i < 8; i++) {
        event.EventType = invalidEventType[i];
        SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
	  status.assertPass();
        } else {
	  status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD, SA_ERR_HPI_INVALID_PARAMS, error);
        }
      }
    } else {
      status.assertNotSupport();
    }
    return status;
}


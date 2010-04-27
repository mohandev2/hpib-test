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

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidEventType::InvalidEventType(char *line) : HpiSessionTestCase(line) {
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
    return "The event type is not SAHPI_ET_USER but the <i>Source</i> field\n"
           "is SAHPI_UNSPECIFIED_RESOURCE_ID.";
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

HpiTestStatus InvalidEventType::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventTypeT et[] = { SAHPI_ET_RESOURCE, SAHPI_ET_DOMAIN,
                             SAHPI_ET_SENSOR, SAHPI_ET_SENSOR_ENABLE_CHANGE,
                             SAHPI_ET_HOTSWAP, SAHPI_ET_WATCHDOG,
                             SAHPI_ET_HPI_SW, SAHPI_ET_OEM };

    EventHelper::fill(&event);
    for (int i = 0; i < 8; i++) {
        event.EventType = et[i];
        SaErrorT error = saHpiEventAdd(sessionId, &event);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_ADD, SA_ERR_HPI_INVALID_PARAMS, error, 
                                 "EventType: %s", HpiString::eventType(et[i]));
        }
    }

    return status;
}


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

#include "NoRecord.h"
#include "EventHelper.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoRecord::NoRecord(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoRecord::getName() {
    return "NoRecord";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoRecord::getDescription() {
    return "If an HPI User provides a pointer for an RDR, but there is no\n"
           "valid RDR associated with the event being returned (e.g. a\n"
           "User Event), then the <i>Rdr->RdrType</i> will be set to\n"
           "SAHPI_NO_RECORD.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoRecord::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add a user event and then wait for that event to show up in the
 * event queue.  The RdrType must be SAHPI_NO_RECORD.
 *****************************************************************************/

HpiTestStatus NoRecord::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT userEvent;
    SaHpiRdrT rdr;
    bool foundEvent;

    EventHelper::fill(&userEvent);
    SaErrorT error = saHpiEventAdd(sessionId, &userEvent);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_ADD, SA_OK, error);
    } else {
        status.add(TRACE, EventHelper::waitForEvent(sessionId, &userEvent,
                                                    &rdr, NULL, NULL,
                                                    &foundEvent));
        if (status.isOkay()) {
            if (!foundEvent) {
                status.assertError(TRACE, "The user event that was added was "
                                          "never found in the event queue.");
            } else if (rdr.RdrType != SAHPI_NO_RECORD) {
                status.assertFailure(TRACE, 
                    "User event's RdrType is %s instead of SAHPI_NO_RECORD.",
                    HpiString::rdrType(rdr.RdrType));
            } else {
                status.assertPass();
            }
        }
    }

    return status;
}

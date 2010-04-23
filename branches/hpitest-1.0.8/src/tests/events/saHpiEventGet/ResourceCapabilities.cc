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

#include "ResourceCapabilities.h"
#include "EventHelper.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ResourceCapabilities::ResourceCapabilities(char *line) : SubscribeTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ResourceCapabilities::getName() {
    return "ResourceCapabilities";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ResourceCapabilities::getDescription() {
    return "If an HPI User provides a pointer for an RPT entry, but the\n"
           "event does not include a valid ResourceId for a resource in the\n"
           "domain (e.g. a USER type event), then the\n"
           "<i>RptEntry->ResourceCapabilities</i> field will be set to zero.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ResourceCapabilities::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ResourceCapabilities::runEventTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT userEvent;
    SaHpiRptEntryT rptEntry;
    bool foundEvent;

    EventHelper::fill(&userEvent);
    SaErrorT error = saHpiEventAdd(sessionId, &userEvent);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_ADD, SA_OK, error);
    } else {
        status.add(TRACE, EventHelper::waitForEvent(sessionId, &userEvent, NULL,
                                                    &rptEntry, NULL, &foundEvent));
        if (status.isOkay()) {
            if (!foundEvent) {
                status.assertError(TRACE, "The user event that was added was "
                                          "never found in the event queue.");
            } else if (rptEntry.ResourceCapabilities == 0) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, "User event's RptEntry->ResourceCapabilities "
                                     "is 0x%X instead of zero.", 
                                     rptEntry.ResourceCapabilities);
            }
        }
    }

    return status;
}

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

#include "FruExtraction.h"
#include "EventHelper.h"

using namespace ns_saHpiResourceSeveritySet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FruExtraction::FruExtraction(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *FruExtraction::getName() {
    return "FruExtraction";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *FruExtraction::getDescription() {
    return "When a resource is no longer present in the system, i.e. a\n"
           "surprise extraction has occurred, the generated event will be\n"
           "assigned a severity that can be set via saHpiResourceSeveritySet().";
}

/*****************************************************************************
 * This is a manual test.
 *****************************************************************************/

bool FruExtraction::isManual() {
    return true;
}

/*****************************************************************************
 * Run the test.
 *
 * We must first ask the user to insert a FRU so that we can identify that
 * FRU from other resources.  Once found, we can change it's severity and
 * then have the user to remove the FRU.  The generated event must have the
 * same severity that we assigned.
 *****************************************************************************/

HpiTestStatus FruExtraction::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId;
    SaHpiSeverityT severity;
    SaHpiSeverityT newSeverity;

    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        status.add(TRACE, insertFRU(sessionId, &resourceId, &severity));
        if (status.isOkay()) {

            // change the severity
            if (severity == SAHPI_CRITICAL) {
                newSeverity = SAHPI_MAJOR;
            } else {
                newSeverity = SAHPI_CRITICAL;
            }

            error = saHpiResourceSeveritySet(sessionId, resourceId, newSeverity);
            if (error != SA_OK) {
                status.assertError(TRACE, RESOURCE_SEVERITY_SET, SA_OK, error);
            } else {
                status.add(TRACE, removeFRU(sessionId, resourceId, newSeverity));

                // NOTE: Cannot restore the severity since the FRU was extracted,
                //       i.e. it is no longer present.
            }
        }

        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}

/*****************************************************************************
 * Have the user insert an FRU.  This is only done in order to identify
 * the resource.
 *****************************************************************************/

HpiTestStatus FruExtraction::insertFRU(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT *resourceId,
                                       SaHpiSeverityT *severity) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiRptEntryT rptEntry;
    bool foundEvent;

    promptInsertFru(sessionId);

    EventMatcher matcher;
    matcher.setEventType(SAHPI_ET_HOTSWAP);
    matcher.addHotSwapState(SAHPI_HS_STATE_ACTIVE);
    matcher.addHotSwapState(SAHPI_HS_STATE_INSERTION_PENDING);
    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, &event, 
                                                NULL, &rptEntry, NULL, &foundEvent));
    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertError(TRACE, "Did not receive a HOT SWAP "
                                      "event in the ACTIVE state.");
        } else {
            *resourceId = rptEntry.ResourceId;
            *severity = rptEntry.ResourceSeverity;
        }
    }

    return status;
}

/*****************************************************************************
 * Have the user remove the same FRU that was inserted.  Wait for a hot swap
 * event and verify that's severity matched the severty that we set it to.
 *****************************************************************************/

HpiTestStatus FruExtraction::removeFRU(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId,
                                       SaHpiSeverityT severity) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiRptEntryT rptEntry;
    bool foundEvent;

    promptFru(sessionId, "In order to cause a \"surprise extraction\", please "
                         "remove the same FRU that was previously inserted.");

    EventMatcher matcher;
    matcher.setSource(resourceId);
    matcher.setEventType(SAHPI_ET_RESOURCE);
    matcher.setResourceEventType(SAHPI_RESE_RESOURCE_FAILURE);
    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, &event, 
                                                NULL, &rptEntry, NULL, &foundEvent));
    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertError(TRACE, "Did not receive a HOT SWAP event in the "
                                      "NOT PRESENT state for the FRU.");
        } else if (event.Severity != severity) {
            status.assertFailure(TRACE, "Expected the Hot Swap event to have severity %s, "
                                        "but %s was found instead.",
                                        HpiString::severity(severity),
                                        HpiString::severity(event.Severity));
        } else {
            status.assertPass();
        }
    }

    return status;
}

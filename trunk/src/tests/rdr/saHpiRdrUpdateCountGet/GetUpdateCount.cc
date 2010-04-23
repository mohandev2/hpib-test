/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 */

#include "GetUpdateCount.h"
#include "ResourcesList.h"
#include "EventHelper.h"

using namespace ns_saHpiRdrUpdateCountGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetUpdateCount::GetUpdateCount(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetUpdateCount::getName() {
    return "GetUpdateCount";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetUpdateCount::getDescription() {
    return "Calls to saHpiRdrUpdateCount() for each resource on startup.\n"
           "After receiving the event SAHPI_RESE_RESOURCE_RESTORED or \n"
           "SAHPI_RESE_RESOURCE_UPDATED, the test will call\n"
           "saHpiRdrUpdateCount() again and compare both values.\n"
           "The test case pass if the update count is increased.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetUpdateCount::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * This is a manual test.
 *****************************************************************************/

bool GetUpdateCount::isManual() {
    return true;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetUpdateCount::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
	
	// Get all Update Counts
	ResourcesList origValues;
	origValues.fill(sessionId);
	
    // pause in order to avoid events from previous test cases.
    pauseForEventProcessing();

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
    		SaHpiResourceIdT updResourceId;
    		SaHpiUint32T oldUpdCnt;
    		SaHpiUint32T newUpdCnt;
    			
    		// Now wait for the event
        status.add(TRACE, updateFRU(sessionId, &updResourceId));
        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);

		// Check the old value with the new one        
		error = saHpiRdrUpdateCountGet(sessionId, updResourceId, &newUpdCnt);
        if (error != SA_OK) {
        		status.assertFailure(TRACE, RDR_UPDATE_COUNT_GET, SA_OK, error);
		}
		error = origValues.getUpdateCount(updResourceId, &oldUpdCnt);
		if (error != SA_OK) {
        		status.assertFailure(TRACE, RDR_UPDATE_COUNT_GET, SA_OK, error);
		}
		if (oldUpdCnt >= newUpdCnt) {
			status.assertFailure(TRACE, 	"The Rdr Update Count wasn't increased (old %d - new %d)",
			oldUpdCnt, newUpdCnt);
		}
    }

    return status;
}

/*****************************************************************************
 * Have the user insert an FRU.  Wait for one of the resource events where the 
 * state is SAHPI_RESE_RESOURCE_RESTORED or SAHPI_RESE_RESOURCE_UPDATED.
 * Once that occurs, call saHpiRdrUpdateCountGet again for that resource.  
 *****************************************************************************/

HpiTestStatus GetUpdateCount::updateFRU(SaHpiSessionIdT sessionId, SaHpiResourceIdT *updatedResourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiRptEntryT rptEntry;
    bool foundEvent;

    promptFru(sessionId, "Trigger a change of the Rdr for a resource, please.");

    EventMatcher matcher;
    matcher.setEventType(SAHPI_ET_RESOURCE);
    matcher.setResourceEventType(SAHPI_RESE_RESOURCE_UPDATED);
    matcher.addResourceEventType(SAHPI_RESE_RESOURCE_RESTORED);
    status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, &event, 
                                                NULL, &rptEntry, NULL, &foundEvent));
    if (status.isOkay()) {
        if (!foundEvent) {
            status.assertError(TRACE, "Did not receive a RESOURCE "
                                      "event with the type UPDATED or RESTORED.");
        } else {
        		*updatedResourceId = rptEntry.ResourceId;
	        status.assertPass();
        }
    }

    return status;
}


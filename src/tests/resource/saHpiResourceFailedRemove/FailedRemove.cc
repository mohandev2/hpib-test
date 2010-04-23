/*      
 * (C) Copyright Emerson Corp. 2009
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Lars Wetzel   <Lars.Wetzel@emerson.com>
 */

#include "FailedRemove.h"
#include "EventHelper.h"

// Time in sec, how long it should be allowed to wait for the removal
// of the rpt entry
#define WAITTIME 1

using namespace ns_saHpiResourceFailedRemove;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FailedRemove::FailedRemove(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *FailedRemove::getName() {
    return "FailedRemove";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *FailedRemove::getDescription() {
  return "A failed resource is removed from the RPT with function."
    "saHpiResourceFailedRemove(). Afterwards it is checked with "
    "function saHpiRptEntryGetByResourceId() if the rpt entry is "
    "really deleted.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *FailedRemove::getPrecondition() {
    return "Requires one or more resources which are marked as failed.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT FailedRemove::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus FailedRemove::runResourceTest(SaHpiSessionIdT sessionId,
					    SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    bool foundEvent;
    SaHpiEventT event;
    EventMatcher matcher;

    matcher.setEventType(SAHPI_ET_HOTSWAP);
    matcher.setHotSwapState(SAHPI_HS_STATE_NOT_PRESENT);

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
	return status;
    } 

    // Precondition is a failed Resource
    if (rptEntry->ResourceFailed) {

      error = saHpiResourceFailedRemove(sessionId, rptEntry->ResourceId);
      if (error != SA_OK) {

	status.assertFailure(TRACE, RESOURCE_FAILED_REMOVE,
			     SA_OK, error);
	return status;
      }
      
      status.add(TRACE, EventHelper::waitForEvent(sessionId, matcher, &event, 
                                                NULL, NULL, NULL, &foundEvent));
      if (status.isOkay()) {
        if (!foundEvent) {
	  status.assertError(TRACE, "Did not receive the HOT SWAP event NOT_PRESENT");
	  
        } else {
	  sleep(WAITTIME);

	  // The resource should be removed from the Rpt
	  error = saHpiRptEntryGetByResourceId(sessionId,
					       rptEntry->ResourceId,
					       rptEntry);
	  if (error == SA_ERR_HPI_INVALID_RESOURCE) {
	    status.assertPass();

	  } else {
	    status.assertFailure(TRACE, 
				 "Valid ResourceId exists although the resource should be removed",
				 error);
	  }
	}  
      }
    } else {

      status.assertNotSupport();
    }

    error = saHpiUnsubscribe(sessionId);
    status.checkError(TRACE, UNSUBSCRIBE, error);
    
    return status;
}

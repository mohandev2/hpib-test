/*
 * (C) Copyright Emerson Network Power, 2009
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
 *     Lars Wetzel <Lars.Wetzel@emerson.com>
 */

#include "MissingCapability.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MissingCapability::MissingCapability(char *line)
  :  EventLogTestCase(line) {
}
/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *MissingCapability::getName() {
    return "MissingCapability";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *MissingCapability::getDescription() {
  return "Try to set the state of a Event Log which doesn't have the capability\n";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT MissingCapability::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

const char *MissingCapability::getPrecondition() {
  return "Requires a resource that supports Event Logs but not to set the state.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus MissingCapability::runLogTest(SaHpiSessionIdT sessionId,
					    SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiBoolT savestate;

    if (!EventLogHelper::hasEvtLogStateSetCapability(sessionId, resourceId)) {

      SaErrorT error = saHpiEventLogStateGet(sessionId, resourceId, &savestate);
      if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
        return status;
      }

      error = saHpiEventLogStateSet(sessionId, resourceId, savestate);
      if (error == SA_ERR_HPI_INVALID_CMD) {
	
	status.assertPass();
      } else {

	status.assertFailure(TRACE, EVENT_LOG_STATE_SET, 
			     SA_ERR_HPI_INVALID_CMD, error);
      }
    } else {
      
      status.assertNotSupport();
    }
    
    return status;
}


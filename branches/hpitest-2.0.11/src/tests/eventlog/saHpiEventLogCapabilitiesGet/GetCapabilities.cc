/*      
 * (C) Copyright IBM Corp. 2008
 * (C) Copyright Emerson Network Power 2009
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
 *  Suntrupth S Yadav <suntrupth@in.ibm.com>
 *  Lars Wetzel <Lars.Wetzel@emerson.com>
 *
 * Changes     
 * 2009/05/19 - Lars.Wetzel@emerson.com
 *              Verify if proper capability is set
 *              Test statement from the remarks section
 */

#include "GetCapabilities.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogCapabilitiesGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetCapabilities::GetCapabilities(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetCapabilities::getName() {
    return "GetCapabilities";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetCapabilities::getDescription() {
    return "Retrieve the Event Log Capabilities for the domain and the\n"
           "resources that support Event Logs.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetCapabilities::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetCapabilities::runLogTest(SaHpiSessionIdT sessionId,
					  SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogCapabilitiesT eventLogCapabilities;
    SaHpiEventLogInfoT eventLogInfo;

    SaErrorT error = saHpiEventLogCapabilitiesGet(sessionId, resourceId, &eventLogCapabilities);
    if (error == SA_OK) {
       if (!EventLogHelper::isValidCapability(eventLogCapabilities)) {
	status.assertFailure(TRACE, "The returned Event Log Capability is invalid [0x%X]", 
			     eventLogCapabilities);
      }
      
      error = saHpiEventLogInfoGet(sessionId, resourceId, &eventLogInfo);
      if (error != SA_OK) {
	status.assertFailure(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
      
      } else {
	// see also Remarks in the specification: 
	// Cap OVERFLOW_RESETABLE == eventLogInfo.OverflowResetable
	if (((eventLogCapabilities & SAHPI_EVTLOG_CAPABILITY_OVERFLOW_RESET) &&
	    eventLogInfo.OverflowResetable) ||
	    (!(eventLogCapabilities & SAHPI_EVTLOG_CAPABILITY_OVERFLOW_RESET) &&
	     !eventLogInfo.OverflowResetable)) {
	  status.assertPass();
	  
	} else {
	  status.assertFailure(TRACE, 
			       "SAHPI_EVTLOG_CAPABILITY_OVERFLOW_RESET capability != eventLogInfo.OverflowResetable (%d)",
			       eventLogInfo.OverflowResetable);
	}
	
      }
    } else {
      status.assertFailure(TRACE, EVENT_LOG_CAPABILITIES_GET, SA_OK, error);
    } 

    return status;
}


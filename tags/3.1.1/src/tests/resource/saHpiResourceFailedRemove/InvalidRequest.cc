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

#include "InvalidRequest.h"

using namespace ns_saHpiResourceFailedRemove;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidRequest::InvalidRequest(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidRequest::getName() {
    return "InvalidRequest";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidRequest::getDescription() {
  return "saHpiResourceFailedRemove() is called for a resource which is not "
    "marked as failed.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidRequest::getPrecondition() {
    return "Requires one or more resources which are not marked as failed.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidRequest::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidRequest::runResourceTest(SaHpiSessionIdT sessionId,
						     SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    // Precondition - Resource is not failed
    if (rptEntry->ResourceFailed) {
      status.assertNotSupport();

    } else {
     
      // Test case
      SaErrorT error = saHpiResourceFailedRemove(sessionId,
						 rptEntry->ResourceId);
      if (error != SA_ERR_HPI_INVALID_REQUEST) {

	status.assertFailure(TRACE, RESOURCE_FAILED_REMOVE,
			     SA_ERR_HPI_INVALID_REQUEST, error);
      }
       
      // Control that the resource still exists in the system
      error = saHpiRptEntryGetByResourceId(sessionId,
					   rptEntry->ResourceId,
					   rptEntry);
      
      if (error != SA_OK) {

	status.assertError(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID,
			   SA_OK, error);
      } else {
	status.assertPass();
      }
    }
	
    return status;
}

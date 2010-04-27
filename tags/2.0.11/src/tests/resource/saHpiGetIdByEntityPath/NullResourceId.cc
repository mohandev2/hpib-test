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
#include "NullResourceId.h"

using namespace ns_saHpiGetIdByEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullResourceId::NullResourceId(char *line) : HpiResourceTestCase(line) {
  fastExit = false;
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullResourceId::getName() {
    return "NullResourceId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullResourceId::getDescription() {
    return "The <i>ResourceId</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullResourceId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullResourceId::runResourceTest(SaHpiSessionIdT sessionId,
					      SaHpiRptEntryT *rptEntry) {
  HpiTestStatus      status;
  SaHpiEntryIdT      nextEntryId = SAHPI_FIRST_ENTRY;
  SaHpiEntryIdT      entryId;
  SaHpiRdrT          rdr;
  SaErrorT           error;

  SaHpiUint32T       instanceId = SAHPI_FIRST_ENTRY;
  SaHpiInstrumentIdT instrumentId;
  SaHpiUint32T       rptUpdateCount;

  if (!hasRdrCapability(rptEntry)) {
    status.assertNotSupport();
      
  } else {
      
    while ((nextEntryId != SAHPI_LAST_ENTRY) && !fastExit){
      entryId = nextEntryId;
      error = saHpiRdrGet(sessionId, rptEntry->ResourceId, entryId, &nextEntryId, &rdr);
      if (error != SA_OK) {
	status.assertError(TRACE, RDR_GET, SA_OK, error);

      } else {
	  
	// 2. Check ResourceId == NULL
	error = saHpiGetIdByEntityPath(sessionId, rdr.Entity, rdr.RdrType, 
				       &instanceId, NULL, &instrumentId, &rptUpdateCount );
	if (error != SA_ERR_HPI_INVALID_PARAMS) {
	  status.assertFailure(TRACE, GET_ID_BY_ENTITY_PATH, SA_ERR_HPI_INVALID_PARAMS, error);
	  break;
	}
	status.assertPass();
	fastExit = true;
	break;
      }
    }
  }
  return status;
}


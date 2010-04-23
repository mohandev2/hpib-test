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

#include "NullInstrumentId.h"

using namespace ns_saHpiGetIdByEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullInstrumentId::NullInstrumentId(char *line) : HpiResourceTestCase(line) {
  fastExit = false;
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullInstrumentId::getName() {
    return "NullInstrumentId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullInstrumentId::getDescription() {
    return "The specific management instrument identified in the\n"
           "<i>InstrumentId</i> parameter is not present in the addressed\n"
           "resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullInstrumentId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullInstrumentId::runResourceTest(SaHpiSessionIdT sessionId,
						   SaHpiRptEntryT *rptEntry) {
  HpiTestStatus      status;
  SaHpiEntryIdT      nextEntryId = SAHPI_FIRST_ENTRY;
  SaHpiEntryIdT      entryId;
  SaHpiRdrT          rdr;
  SaErrorT           error;

  SaHpiUint32T       instanceId = SAHPI_FIRST_ENTRY;
  SaHpiResourceIdT   resourceId;
  SaHpiUint32T       rptUpdateCount;

  if (!hasRdrCapability(rptEntry)) {
    status.assertNotSupport();
    
  } else {
      
    while ((nextEntryId != SAHPI_LAST_ENTRY) && !fastExit){
      entryId = nextEntryId;
      error = saHpiRdrGet(sessionId, rptEntry->ResourceId, entryId, &nextEntryId, &rdr);
      if (error != SA_OK) {
	status.assertError(TRACE, RDR_GET, SA_OK, error);
	
      } else if (rdr.RdrType == SAHPI_NO_RECORD) {
	status.assertNotSupport();
	
      } else {

	// 4. Check InstrumentId == NULL &&  rdr.RdrType != SAHPI_NO_RECORD
	error = saHpiGetIdByEntityPath(sessionId, rdr.Entity, rdr.RdrType, 
				       &instanceId, &resourceId, NULL, &rptUpdateCount );
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

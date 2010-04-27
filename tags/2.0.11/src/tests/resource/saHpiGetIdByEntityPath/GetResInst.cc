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
 *
 *  Changes:
 *  09/06/02  lars.wetzel@emerson.com
 *           - Add NotSupport (for SA_ERR_HPI_NOT_PRESENT) after review
 *  09/08/28  lars.wetzel@emerson.com
 *           - Add test for SA_HPI_NO_RECORD
 */

#include "GetResInst.h"
#include "Report.h"
#include "RdrHelper.h"

using namespace ns_saHpiGetIdByEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetResInst::GetResInst(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetResInst::getName() {
    return "GetResInst";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetResInst::getDescription() {
  return "Get <i>ResourceId</i> <i>InstrumentId</i> by valid \n"
    "entity path and rdr type combination";

}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetResInst::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetResInst::runResourceTest(SaHpiSessionIdT sessionId,
					  SaHpiRptEntryT *rptEntry) {
  HpiTestStatus      status;
  SaErrorT           error;

  SaHpiUint32T       instanceId = SAHPI_FIRST_ENTRY;
  SaHpiResourceIdT   resourceId;
  SaHpiUint32T       rptUpdateCount;
  SaHpiInstrumentIdT instrumentId = 0;

  SaHpiCapabilitiesT capability[] = { SAHPI_CAPABILITY_CONTROL,
				      SAHPI_CAPABILITY_SENSOR,
				      SAHPI_CAPABILITY_INVENTORY_DATA,
				      SAHPI_CAPABILITY_ANNUNCIATOR,
				      SAHPI_CAPABILITY_WATCHDOG,
				      SAHPI_CAPABILITY_DIMI,
				      SAHPI_CAPABILITY_FUMI };

  SaHpiRdrTypeT rdrType[] = { SAHPI_CTRL_RDR,
			      SAHPI_SENSOR_RDR,
			      SAHPI_INVENTORY_RDR, 
			      SAHPI_ANNUNCIATOR_RDR,
			      SAHPI_WATCHDOG_RDR,
			      SAHPI_DIMI_RDR,
			      SAHPI_FUMI_RDR };

  // Default value (get ResourceId without InstrumentType)
  error = saHpiGetIdByEntityPath(sessionId, rptEntry->ResourceEntity, SAHPI_NO_RECORD, 
				 &instanceId, &resourceId, NULL, &rptUpdateCount );
  if (error != SA_OK) {
    status.assertFailure(TRACE, "saHpiGetIdByEntityPath returns %s instead of SA_OK "
			 "\nentity path: %s"
			 "\nInstrumentType SA_HPI_NO_RECORD, InstrumentId NULL\n",
			 HpiString::error(error),
			 HpiString::entityPath(& rptEntry->ResourceEntity));
    //    status.assertFailure(TRACE, GET_ID_BY_ENTITY_PATH, SA_OK, error);
  
  } else if ( resourceId != rptEntry->ResourceId ) {
    status.assertFailure(TRACE, "saHpiGetIdByEntityPath returns wrong ResourceId %d for resource:"
				 "\nEntity Path %s"
			         "\nResourceId  %d"
				 "\nRdrType     SAHPI_NO_RECORD",
			         resourceId,
			         rptEntry->ResourceId,
				 HpiString::entityPath(& rptEntry->ResourceEntity));
  }

  if (hasRdrCapability(rptEntry)) {
     for (int i = 0; i < 7; i++) {
      if (hasCapability(rptEntry, capability[i])) {
	    
	instanceId = SAHPI_FIRST_ENTRY;
	while (instanceId != SAHPI_LAST_ENTRY) {
	  error = saHpiGetIdByEntityPath(sessionId, rptEntry->ResourceEntity, rdrType[i], 
					 &instanceId, &resourceId, &instrumentId, &rptUpdateCount );
	  if (error == SA_ERR_HPI_NOT_PRESENT) {
	    status.assertNotSupport();

	  } else if (error != SA_OK) {
	    status.assertFailure(TRACE, "saHpiGetIdByEntityPath returns %s for valid combination:"
				 "\nEntity Path %s"
				 "\nRdrType     %s",
				 HpiString::error(error),
				 HpiString::entityPath(& rptEntry->ResourceEntity),
				 HpiString::rdrType(rdrType[i]));
	    break;

	  } else {
	    	    
	    SaHpiRdrT checkRdr;
	    Report report;
	    error = saHpiRdrGetByInstrumentId(sessionId, resourceId, rdrType[i], instrumentId, &checkRdr);
	    if (error != SA_OK) {
	      status.assertFailure(TRACE, RDR_GET_BY_INSTRUMENT_ID, SA_OK, error);
	      break;

	    } else if (!RdrHelper::isValid(&checkRdr, report)) {
	      status.assertFailure(TRACE, report.toString());
	      break;
  
	    } else {
	      status.assertPass();

	    }
	  }
	}
      }
    }
  }
  
  return status;
}


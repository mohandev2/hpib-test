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

#include "ResInstNotPresent.h"

using namespace ns_saHpiGetIdByEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ResInstNotPresent::ResInstNotPresent(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ResInstNotPresent::getName() {
    return "ResInstNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ResInstNotPresent::getDescription() {
    return "The specific management instrument identified in the\n"
           "<i>InstrumentId</i> parameter is not present in the addressed\n"
           "resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ResInstNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ResInstNotPresent::runResourceTest(SaHpiSessionIdT sessionId,
						 SaHpiRptEntryT *rptEntry) {
  HpiTestStatus      status;
  SaErrorT           error;

  SaHpiUint32T       instanceId = SAHPI_FIRST_ENTRY;
  SaHpiResourceIdT   resourceId;
  SaHpiUint32T       rptUpdateCount;
  SaHpiInstrumentIdT instrumentId;
 
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
  
  if (!hasRdrCapability(rptEntry)) {
    status.assertNotSupport();
    
  } else {

    for (int i = 0; i < 7; i++) {
      if (!hasCapability(rptEntry, capability[i])) {
	    
	instanceId = SAHPI_FIRST_ENTRY;
	error = saHpiGetIdByEntityPath(sessionId, rptEntry->ResourceEntity, rdrType[i], 
				       &instanceId, &resourceId, &instrumentId, &rptUpdateCount );

	if (error == SA_ERR_HPI_NOT_PRESENT) {
	  status.assertPass();
	  
	} else if (error != SA_OK) {
	  status.assertFailure(TRACE, GET_ID_BY_ENTITY_PATH, SA_ERR_HPI_NOT_PRESENT, error);
	  break;

	} else {

	  SaHpiRptEntryT checkRpt;
	  error = saHpiRptEntryGetByResourceId(sessionId, resourceId, &checkRpt);
	  if (error != SA_OK) {
	    status.assertFailure(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
	    break;

	  } 
	  if (hasCapability(&checkRpt, capability[i])) {
	    status.assertNotSupport();

	  } else {
	    status.assertFailure(TRACE, "Returned resourceId doesn't have the capability for rdrType %s",
				 HpiString::rdrType(rdrType[i]));
	  }
	}
      }
    }
  }

  return status;
}


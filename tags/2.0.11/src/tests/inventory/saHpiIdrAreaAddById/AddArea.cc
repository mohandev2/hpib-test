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
 *     Akmal A. Muqeeth <aax6@unh.edu>
 *
 * Changes
 * 09/06/25 lars.wetzel@emerson.com
 *          areaType: PRODUCT_INFO -> OEM
 *
 * 09/07/29 lars.wetzel@emerson.com
 *          valid for READWRITE_IDR
 */

#include "AddArea.h"

using namespace ns_saHpiIdrAreaAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddArea::AddArea(char *line) : WriteIdrTestCase(line, WRITE_TC_READWRITE_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddArea::getName() {
    return "AddAreaById";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddArea::getDescription() {
    return "Add a new area with a specified Area ID.  Verify that it can be retrieved and\n"
           "that it's <i>AreaHeader.ReadOnly</i> field is False.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AddArea::getPrecondition() {
    return "Requires an Inventory for which an Area can be added by Id.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddArea::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddArea::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiInventoryRecT *idrRec) {
    HpiTestStatus       status;
    SaHpiEntryIdT       areaId = SAHPI_FIRST_ENTRY;
    SaHpiEntryIdT       nextAreaId;
    SaHpiIdrAreaHeaderT header;
    SaHpiIdrAreaTypeT   areaType = SAHPI_IDR_AREATYPE_OEM;
 
    SaErrorT error = saHpiIdrAreaAddById(sessionId,
					 rptEntry->ResourceId, idrRec->IdrId, 
					 areaType, areaId);
 
     if (error == SA_ERR_HPI_INVALID_DATA) {
         status.assertNotSupport();
     } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
         status.assertNotSupport();
     } else if (error != SA_OK) {
        status.assertFailure(TRACE, IDR_AREA_ADD_BY_ID, SA_OK, error);
     } else {
          error = saHpiIdrAreaHeaderGet(sessionId,
                                        rptEntry->ResourceId, idrRec->IdrId,
                                        SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                        areaId, &nextAreaId, &header);
 
        if (error != SA_OK) {
	  status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
 
	} else {

	  areaId = header.AreaId;
	  error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId, idrRec->IdrId, areaId);

	  if (error != SA_OK) {
	    status.assertError(TRACE, IDR_AREA_DELETE, SA_OK, error);

	  } else {  
	    status.assertPass();
	  }

	}
     }






    return status;
}


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
 *  Akmal A. Muqeeth <aax6@unh.edu>
 *
 * Changes
 * 09/06/25 lars.wetzel@emerson.com
 *          deleteArea: false -> true
 *          small fixes and editorial change 
 *
 * 09/07/30 anurag.saxena@emerson.com
 *          lars.wetzel@emerson.com
 *          changed to run on read-only IDR also
 *          -> change parent class and re-write test case
 */

#include "DuplicateField.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DuplicateField::DuplicateField(char *line) : WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DuplicateField::getName() {
    return "DuplicateField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DuplicateField::getDescription() {
    return "Check if Field represented by the FieldId is already present in the IDR or not.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DuplicateField::getPrecondition() {
    return "Requires a read-only or read-write Inventory with a read-write\n"
           "Area for which free space is available and the new field can be\n"
           "added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DuplicateField::getExpectedReturn() {
    return SA_ERR_HPI_DUPLICATE;
}

/*****************************************************************************
 * Run the test.
 * Add a field to the area.
 * Then try to add the same field again using FieldAddById, since the field
 * already exists it should return SA_ERR_HPI_DUPLICATE.
 * The field is deleted with the area after the run of this test case. 
 * (see also parent class AddIdrTestCase)
 *****************************************************************************/

HpiTestStatus DuplicateField::runWriteIdrTest(SaHpiSessionIdT sessionId,
					      SaHpiRptEntryT *rptEntry,
					      SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;

    while (nextAreaId != SAHPI_LAST_ENTRY && !status.hasFault()) {
      areaId = nextAreaId;
      SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
					     rptEntry->ResourceId,
					     idrRec->IdrId,
					     SAHPI_IDR_AREATYPE_UNSPECIFIED,
					     areaId, &nextAreaId, &header);

      if ((areaId == SAHPI_FIRST_ENTRY) && (error == SA_ERR_HPI_NOT_PRESENT) ) {
        status.assertNotSupport();
        break;
      }
      if (error != SA_OK) {
	status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);

      } else if (!header.ReadOnly) {

	// In this area FieldAdd should be allowed
        InventoryHelper::fill(&field, areaId);
        error = saHpiIdrFieldAdd(sessionId, rptEntry->ResourceId, idrRec->IdrId, &field);
        if (error == SA_ERR_HPI_INVALID_DATA) {
	  status.assertNotSupport();
        } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
	  status.assertNotSupport();
        } else if (error != SA_OK) {
	  status.assertError(TRACE, IDR_FIELD_ADD, SA_OK, error);
        } else {
	  
	  // Now, the same field will be added
	  error = saHpiIdrFieldAddById(sessionId, rptEntry->ResourceId,
				       idrRec->IdrId, &field);
	  if (error == SA_ERR_HPI_DUPLICATE) {
	    status.assertPass();
	  } else {
	    status.assertFailure(TRACE, IDR_FIELD_ADD_BY_ID,
				 SA_ERR_HPI_INVALID_PARAMS, error);
	  }
	    
	  // Remove the field
	  error = saHpiIdrFieldDelete(sessionId, rptEntry->ResourceId,
				      idrRec->IdrId, areaId, field.FieldId);
	  if (error != SA_OK) {
	    status.assertError(TRACE, IDR_FIELD_DELETE, SA_OK, error);
	  }
        }

      } else {
	status.assertNotSupport();
      }
    }

    return status;
}

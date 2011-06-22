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
 *     Donald A. Barre <dbarre@unh.edu>
 *
 * Changes
 * 09/07/30 anurag.saxena@emerson.com
 *          lars.wetzel@emerson.com
 *          valid for WRITE_TC_ALL_IDR
 *          Change test case: 
 *          Existing IDR Fields are tested, instead of adding a new area
 *
 */

#include "SetField.h"
#include "InventoryHelper.h"
#include "Report.h"

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetField::SetField(char *line) 
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetField::getName() {
    return "SetField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetField::getDescription() {
    return "Set a field by changing its FieldType from\n"
           "SAHPI_IDR_FIELDTYPE_CUSTOM to SAHPI_IDR_FIELDTYPE_ASSET_TAG\n"
           "and by changing its Data.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetField::getPrecondition() {
  return "Requires a read-write Field for which a change to the standard\n"
         "values is allowed.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetField::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, set a field's FieldType and Data.
 *****************************************************************************/

HpiTestStatus SetField::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;

    status.assertNotSupport();
    while (nextAreaId != SAHPI_LAST_ENTRY && !status.hasFault()) {
        areaId = nextAreaId;
        SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                               rptEntry->ResourceId,
                                               idrRec->IdrId,
                                               SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                               areaId, &nextAreaId, &header);

	if (error != SA_OK) {

	  // special case: No Area exists in the IDR
	  if ((error == SA_ERR_HPI_NOT_PRESENT) &&
	      (areaId == SAHPI_FIRST_ENTRY))
	    return status;
	      
	  status.assertError(TRACE, IDR_AREA_HEADER_GET,
			     SA_OK, error);

        } else {
	  if (header.NumFields > 0)
            status.add(TRACE, setField(sessionId, rptEntry->ResourceId,
                                       idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * Add a new field to the area and then try to change it's FieldType and
 * Data.
 *****************************************************************************/

HpiTestStatus SetField::setField(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId,
                                 SaHpiIdrIdT idrId,
                                 SaHpiEntryIdT areaId) {
    
    HpiTestStatus status;
    Report frep;
    SaHpiIdrFieldT origfield;
    SaHpiIdrFieldT newField;
    SaHpiIdrFieldT tmpField;
    SaHpiEntryIdT fieldId;
    SaHpiEntryIdT nextFieldId = SAHPI_FIRST_ENTRY;
    SaErrorT error;

    while (nextFieldId != SAHPI_LAST_ENTRY 
	   && !status.hasFault()) {

      fieldId = nextFieldId;
      error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
			       SAHPI_IDR_FIELDTYPE_UNSPECIFIED, fieldId,
			       &nextFieldId, &origfield);

      if (error != SA_OK) {
	status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);

      } else if (origfield.ReadOnly) {
	status.assertNotSupport();

      } else {
	InventoryHelper::fill(&newField, areaId, origfield.FieldId);

	error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &newField);
	if (error == SA_ERR_HPI_INVALID_DATA) {
	  status.assertNotSupport();
	} else if (error != SA_OK) {
	  status.assertError(TRACE, IDR_FIELD_SET, SA_OK, error);
	} else {

	  error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
				   newField.Type, newField.FieldId,
				   &nextFieldId, &tmpField);
	  if (error != SA_OK) {
	    status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
	  } else if (!InventoryHelper::isEqual(&newField, &tmpField, frep)) {
	    status.assertError(TRACE, "Difference in Idr-Field content: %s", frep.toString());
	  }	 

	  // Restore the original values
          error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &origfield);
	  if (origfield.Type > SAHPI_IDR_FIELDTYPE_UNSPECIFIED) {
		  if (error == SA_ERR_HPI_INVALID_PARAMS){
    // As per SaHpi.h it is ok to have this return. 
        status.assertPassWarn(TRACE, "Returned SA_ERR_HPI_INVALID_PARAMS. "
                                     "Field Type is > SAHPI_IDR_FIELDTYPE_UNSPECIFIED.");
		  } else {
			  status.assertError(TRACE, IDR_FIELD_SET,
					  SA_ERR_HPI_INVALID_PARAMS, error);
		  }
	  }
	  else {
		  if (error != SA_OK) {
			  status.assertError(TRACE, IDR_FIELD_SET, SA_OK, error);
		  } else { 
			  status.assertPass();
		  }
	  }
	}
      }

    }

    return status;
}

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
 */

#include "InvalidFieldType.h"

using namespace ns_saHpiIdrFieldGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidFieldType::InvalidFieldType(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidFieldType::getName() {
    return "InvalidFieldType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidFieldType::getDescription() {
    return "The <i>FieldType</i> parameter is not one of the valid\n"
           "enumerated values for this type.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidFieldType::getPrecondition() {
    return "Requires an Inventory with at least one Area.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidFieldType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 * 
 * For each area, get it's first field by using an invalid field type.
 *****************************************************************************/

HpiTestStatus InvalidFieldType::runIdrTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;
    SaHpiEntryIdT nextFieldId;
    SaHpiIdrFieldT field;
    int invalidFieldType = (int) SAHPI_IDR_FIELDTYPE_CUSTOM + 1;

    while (nextAreaId != SAHPI_LAST_ENTRY && status.isOkay()) {
        areaId = nextAreaId;
        SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                               rptEntry->ResourceId,
                                               idrRec->IdrId,
                                               SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                               areaId, &nextAreaId, &header);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET,
                               SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
        } else {
             error = saHpiIdrFieldGet(sessionId,
                                      rptEntry->ResourceId,
                                      idrRec->IdrId,
                                      header.AreaId, 
                                      (SaHpiIdrFieldTypeT) invalidFieldType,
                                      SAHPI_FIRST_ENTRY,
                                      &nextFieldId, &field);

             if (error == SA_ERR_HPI_INVALID_PARAMS) {
                 status.assertPass();
             } else {
                 status.assertFailure(TRACE, IDR_FIELD_GET,
                                      SA_ERR_HPI_INVALID_PARAMS, error);
             }
        }
    }

    return status;
}


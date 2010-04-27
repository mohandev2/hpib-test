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

#include "TraversalByFieldType.h"

using namespace ns_saHpiIdrFieldGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TraversalByFieldType::TraversalByFieldType(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TraversalByFieldType::getName() {
    return "TraversalByFieldType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TraversalByFieldType::getDescription() {
    return "Retrieve all of the fields for an area using a specific field type.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *TraversalByFieldType::getPrecondition() {
    return "Requires an Inventory with at least one Field.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT TraversalByFieldType::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Traverse the fields for each Area in the IDR.  Try all of the different
 * field types.
 *****************************************************************************/

HpiTestStatus TraversalByFieldType::runIdrTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry,
                                               SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;
    SaHpiEntryIdT fieldId;
    SaHpiEntryIdT nextFieldId;
    SaHpiIdrFieldT field;
    SaHpiIdrFieldTypeT fieldType[] = { SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE,
                                       SAHPI_IDR_FIELDTYPE_MFG_DATETIME,
                                       SAHPI_IDR_FIELDTYPE_MANUFACTURER,
                                       SAHPI_IDR_FIELDTYPE_PRODUCT_NAME,
                                       SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION,
                                       SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER,
                                       SAHPI_IDR_FIELDTYPE_PART_NUMBER,
                                       SAHPI_IDR_FIELDTYPE_FILE_ID,
                                       SAHPI_IDR_FIELDTYPE_ASSET_TAG,
                                       SAHPI_IDR_FIELDTYPE_CUSTOM };

    status.assertNotSupport();
    for (int i = 0; i < 10; i++) {
        nextAreaId = SAHPI_FIRST_ENTRY;
        while (nextAreaId != SAHPI_LAST_ENTRY && !status.hasFault()) {
            areaId = nextAreaId;
            SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                                   rptEntry->ResourceId,
                                                   idrRec->IdrId,
                                                   SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                                   areaId, &nextAreaId, &header);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, IDR_AREA_HEADER_GET,
                                   SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
                break;
            } else {
                nextFieldId = SAHPI_FIRST_ENTRY;
                while (nextFieldId != SAHPI_LAST_ENTRY) {
                    fieldId = nextFieldId;
                    error = saHpiIdrFieldGet(sessionId,
                                             rptEntry->ResourceId,
                                             idrRec->IdrId,
                                             header.AreaId, fieldType[i],
                                             fieldId, &nextFieldId, &field);

                    if (error == SA_ERR_HPI_NOT_PRESENT) {
                        break;
                    } else if (error != SA_OK) {
                        status.assertFailure(TRACE, IDR_FIELD_GET, SA_OK, error);
                        break;
                    } else {
                        status.assertPass();
                    }
                }
            }
        }
    }

    return status;
}


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

#include "FieldTypeMismatch.h"

using namespace ns_saHpiIdrFieldGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FieldTypeMismatch::FieldTypeMismatch(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *FieldTypeMismatch::getName() {
    return "FieldTypeMismatch";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *FieldTypeMismatch::getDescription() {
    return "The <i>FieldType</i> parameter is set to a specific field type,\n"
            "but a field matching both the <i>FieldId</i> parameter and the\n"
            "<i>FieldType</i> parameter does not exist.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *FieldTypeMismatch::getPrecondition() {
    return "Requires an Inventory with at lease one Field.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT FieldTypeMismatch::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *
 * Start by finding an actual Field in order to obtain a valid FieldId.
 * The call saHpiIdrFieldGet() with that FieldId but with a different
 * FieldType.
 *****************************************************************************/

HpiTestStatus FieldTypeMismatch::runIdrTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;
    SaHpiEntryIdT nextFieldId;
    SaHpiIdrFieldT field;
    SaHpiIdrFieldTypeT fieldType;

    SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                           rptEntry->ResourceId,
                                           idrRec->IdrId,
                                           SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                           SAHPI_FIRST_ENTRY, 
                                           &nextAreaId, &header);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
    } else {
        error = saHpiIdrFieldGet(sessionId, rptEntry->ResourceId, idrRec->IdrId, 
                                 header.AreaId, SAHPI_IDR_FIELDTYPE_UNSPECIFIED,
                                 SAHPI_FIRST_ENTRY, &nextFieldId, &field);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
        } else {
            if (field.Type == SAHPI_IDR_FIELDTYPE_CUSTOM) {
                fieldType = SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE;
            } else {
                fieldType = SAHPI_IDR_FIELDTYPE_CUSTOM;
            }

            error = saHpiIdrFieldGet(sessionId,
                                     rptEntry->ResourceId, idrRec->IdrId, 
                                     header.AreaId, fieldType,
                                     field.FieldId, &nextFieldId, &field);

            if (error == SA_ERR_HPI_NOT_PRESENT) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, IDR_FIELD_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error);
            }
        }
    }

    return status;
}

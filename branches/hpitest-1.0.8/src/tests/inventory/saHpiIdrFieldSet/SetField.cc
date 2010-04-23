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

#include "SetField.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetField::SetField(char *line) : WriteIdrTestCase(line) {
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
    return "Requires a read-write Inventory with a read-write Area for which\n"
           "free space is available and a new field for testing purposes\n"
           "is not rejected by the implementation, i.e. SA_ERR_HPI_INVALID_DATA.";
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

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET,
                               SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
        } else if (!header.ReadOnly) {
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
    SaHpiIdrFieldT field;
    SaHpiIdrFieldT newField;
    SaHpiEntryIdT nextFieldId;

    InventoryHelper::fill(&field, areaId);
    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, &field);

    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_FIELD_ADD, SA_OK, error);
    } else {

        field.Field.Data[0] = '2';  // change from '1' to '2'
        field.Type = SAHPI_IDR_FIELDTYPE_ASSET_TAG;

        error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &field);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_FIELD_SET, SA_OK, error);
        } else {
            error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
                                     SAHPI_IDR_FIELDTYPE_UNSPECIFIED, field.FieldId, 
                                     &nextFieldId, &newField);
            if (error != SA_OK) {
                status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error, 
                          "The FieldId (0x%X) returned from "
                          "saHpiIdrFieldAdd() is most likely incorrect.",
                          field.FieldId);
            } else if (newField.Field.Data[0] != '2') {
                status.assertFailure(TRACE, "The Field data was not changed.");
            } else if (newField.Type != SAHPI_IDR_FIELDTYPE_ASSET_TAG) {
                status.assertFailure(TRACE, "The Field Type was not changed.");
            } else {
                status.assertPass();
            }
        }

        error = saHpiIdrFieldDelete(sessionId, resourceId, idrId, areaId, field.FieldId);
        status.checkError(TRACE, IDR_FIELD_DELETE, error);
    }

    return status;
}

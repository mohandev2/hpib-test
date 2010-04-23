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

#include "UnspecifiedFieldType.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UnspecifiedFieldType::UnspecifiedFieldType(char *line) : WriteIdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UnspecifiedFieldType::getName() {
    return "UnspecifiedFieldType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UnspecifiedFieldType::getDescription() {
    return "SAHPI_IDR_FIELDTYPE_UNSPECIFIED is not a valid field type.\n"
           "Therefore a field's <i>FieldType</i> should not be changed to\n"
           "SAHPI_IDR_FIELDTYPE_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *UnspecifiedFieldType::getPrecondition() {
    return "Requires a read-write Inventory with a read-write Area for which\n"
           "free space is available and a new field for testing purposes\n"
           "is not rejected by the implementation, i.e. SA_ERR_HPI_INVALID_DATA.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UnspecifiedFieldType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, try to set a field's FieldType to UNSPECIFIED.
 *****************************************************************************/

HpiTestStatus UnspecifiedFieldType::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
 * Add a new field and then try changing its FieldType to UNSPECIFIED.
 *****************************************************************************/

HpiTestStatus UnspecifiedFieldType::setField(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId, 
                                             SaHpiIdrIdT idrId,
                                             SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;

    InventoryHelper::fill(&field, areaId);
    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, &field);

    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_FIELD_ADD, SA_OK, error);
    } else {
        field.Type = SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
        error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &field);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_FIELD_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        } 

        error = saHpiIdrFieldDelete(sessionId, resourceId, idrId, areaId, field.FieldId);
        status.checkError(TRACE, IDR_FIELD_DELETE, error);
    }

    return status;
}

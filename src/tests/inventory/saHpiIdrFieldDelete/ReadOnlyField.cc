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

#include "ReadOnlyField.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnlyField::ReadOnlyField(char *line) : WriteIdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnlyField::getName() {
    return "ReadOnlyField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnlyField::getDescription() {
    return "The field is read-only and does not allow modification.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnlyField::getPrecondition() {
    return "Requires a read-write Inventory with a read-write Area that\n"
           "has a read-only Field.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnlyField::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, try to delete its read-only fields.
 *****************************************************************************/

HpiTestStatus ReadOnlyField::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
            status.add(TRACE, deleteField(sessionId, rptEntry->ResourceId,
                                          idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * For an area, try to delete each of its read-only fields.
 *****************************************************************************/

HpiTestStatus ReadOnlyField::deleteField(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT resourceId, 
                                         SaHpiIdrIdT idrId,
                                         SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;
    SaHpiEntryIdT fieldId;
    SaHpiEntryIdT nextFieldId;

    status.assertNotSupport();
    nextFieldId = SAHPI_FIRST_ENTRY;
    while (nextFieldId != SAHPI_LAST_ENTRY && !status.hasFault()) {
        fieldId = nextFieldId;
        SaErrorT error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
                                          SAHPI_IDR_FIELDTYPE_UNSPECIFIED, fieldId, 
                                          &nextFieldId, &field);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
        } else if (field.ReadOnly) {
            error = saHpiIdrFieldDelete(sessionId, resourceId,
                                        idrId, areaId, field.FieldId);
            if (error == SA_ERR_HPI_READ_ONLY) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, IDR_FIELD_DELETE,
                                     SA_ERR_HPI_READ_ONLY, error);
            }
        }
    }

    return status;
}


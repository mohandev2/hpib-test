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
 *          valid for WRITE_TC_ALL_IDR
 */

#include "ReadOnlyArea.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnlyArea::ReadOnlyArea(char *line) 
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnlyArea::getName() {
    return "ReadOnlyArea";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnlyArea::getDescription() {
    return "The area is read-only and does not allow modifications of its fields.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnlyArea::getPrecondition() {
    return "Requires a read-only or read-write Inventory with a read-only\n"
           "Area with at least one Field.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnlyArea::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *
 * For each read-only area, try deleting a field.
 *****************************************************************************/

HpiTestStatus ReadOnlyArea::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
        } else if (header.ReadOnly) {
            status.add(TRACE, deleteField(sessionId, rptEntry->ResourceId,
                                          idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * Try deleting a field from a read-only area, assuming that the area
 * does have a field.
 *****************************************************************************/

HpiTestStatus ReadOnlyArea::deleteField(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId, 
                                        SaHpiIdrIdT idrId,
                                        SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;
    SaHpiEntryIdT nextFieldId;

    SaErrorT error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
                                      SAHPI_IDR_FIELDTYPE_UNSPECIFIED,
                                      SAHPI_FIRST_ENTRY, 
                                      &nextFieldId, &field);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
    } else {
        error = saHpiIdrFieldDelete(sessionId, resourceId,
                                    idrId, areaId, field.FieldId);
        if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_FIELD_DELETE,
                                 SA_ERR_HPI_READ_ONLY, error);
        }
    }

    return status;
}

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

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnlyField::ReadOnlyField(char *line) 
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
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
    return "Requires a read-only or read-write Inventory with a read-only or\n"
           "read-write Area that has at least one read-only Field.";
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
 * For each area, try to change a read-only field.
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
                                               areaId,
                                               &nextAreaId, &header);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET,
                               SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
//        } else if (!header.ReadOnly) {
        } else {
            status.add(TRACE, setField(sessionId, rptEntry->ResourceId,
                                       idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * For each read-only field in the area, do a pretend set (not really
 * changing anything).
 *****************************************************************************/

HpiTestStatus ReadOnlyField::setField(SaHpiSessionIdT sessionId,
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
            error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &field);
            if (error == SA_ERR_HPI_READ_ONLY) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, IDR_FIELD_SET,
                                     SA_ERR_HPI_READ_ONLY, error);
            }
        }
    }

    return status;
}

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
 * 09/07/28 lars.wetzel@emerson.com
 *          deleteArea is only possible with read-write IDR 
 */

#include "ReadOnlyField.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnlyField::ReadOnlyField(char *line) : 
  WriteIdrTestCase(line, WRITE_TC_READWRITE_IDR) {
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
    return "The IDA contains a read-only field and therefore cannot be deleted.";
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
 * First, we must find an area that is not read-only but that has a read-only
 * field.  If we do, then we can try to delete that area which should fail.
 *****************************************************************************/

HpiTestStatus ReadOnlyField::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    bool found;

    status.add(TRACE, findAreaWithReadOnlyField(sessionId,
                                                rptEntry->ResourceId,
                                                idrRec->IdrId,
                                                &areaId, &found));
    if (status.isOkay()) {
        if (!found) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId,
                                                idrRec->IdrId, areaId);
            if (error == SA_ERR_HPI_READ_ONLY) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, IDR_AREA_DELETE,
                                     SA_ERR_HPI_READ_ONLY, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * If possible, find an area that is not read-only but which has a read-only
 * field.
 *****************************************************************************/

HpiTestStatus ReadOnlyField::findAreaWithReadOnlyField(SaHpiSessionIdT sessionId,
                                                       SaHpiResourceIdT resourceId, 
                                                       SaHpiIdrIdT idrId,
                                                       SaHpiEntryIdT *areaId,
                                                       bool *found) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;
    SaHpiEntryIdT fieldId;
    SaHpiEntryIdT nextFieldId;
    SaHpiIdrFieldT field;

    *found = false;
    while (nextAreaId != SAHPI_LAST_ENTRY && !(*found) && status.isOkay()) {
        *areaId = nextAreaId;
        SaErrorT error = saHpiIdrAreaHeaderGet(sessionId, resourceId, idrId,
                                              SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                              *areaId, &nextAreaId, &header);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
        } else if (!header.ReadOnly) {
            nextFieldId = SAHPI_FIRST_ENTRY;
            while (nextFieldId != SAHPI_LAST_ENTRY) {
                fieldId = nextFieldId;
                error = saHpiIdrFieldGet(sessionId, resourceId, idrId, header.AreaId,
                                         SAHPI_IDR_FIELDTYPE_UNSPECIFIED, fieldId, 
                                         &nextFieldId, &field);
                if (error != SA_OK) {
                    status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
                    break;
                } else if (field.ReadOnly) {
                    *areaId = field.AreaId;
                    *found = true;
                }
            }
        }
    }

    return status;
}

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

#include "VerifyDeletedFields.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

VerifyDeletedFields::VerifyDeletedFields(char *line)
: AddIdrTestCase(line, false) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *VerifyDeletedFields::getName() {
    return "VerifyDeletedFields";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *VerifyDeletedFields::getDescription() {
    return "All fields contained within an area that is deleted are also deleted.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *VerifyDeletedFields::getPrecondition() {
    return "Requires an Inventory that is not read-only and for which\n"
           "an Area and with one Field can be successfully added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT VerifyDeletedFields::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add a field to an area and then delete the area.  Then try to retrieve
 * the field which should fail.
 *****************************************************************************/

HpiTestStatus VerifyDeletedFields::runAddIdrTest(SaHpiSessionIdT sessionId,
                                                 SaHpiRptEntryT *rptEntry, 
                                                 SaHpiInventoryRecT *idrRec,
                                                 SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiEntryIdT nextFieldId;
    SaHpiIdrFieldT field;

    status.add(TRACE, addField(sessionId, rptEntry->ResourceId,
                               idrRec->IdrId, areaId, &field));
    if (status.isOkay()) {
        SaErrorT error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId,
                                            idrRec->IdrId, areaId);
        if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_DELETE, SA_OK, error);
        } else {
            error = saHpiIdrFieldGet(sessionId, rptEntry->ResourceId,
                                     idrRec->IdrId, areaId, 
                                     SAHPI_IDR_FIELDTYPE_UNSPECIFIED,
                                     field.FieldId, 
                                     &nextFieldId, &field);

            if (error == SA_ERR_HPI_NOT_PRESENT) {
                status.assertPass();
            } else if (error == SA_OK) {
                status.assertFailure(TRACE, 
                                "Successfully retrieved a Field (0x%X) \n"
                                "for an Area (0x%X) that was deleted.",
                                field.FieldId, areaId);
            } else {
                status.assertFailure(TRACE, IDR_FIELD_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error);
            }
        }
    }

    return status;
}

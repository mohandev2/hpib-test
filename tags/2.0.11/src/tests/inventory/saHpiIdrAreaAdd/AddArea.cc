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
 * 09/06/25 lars.wetzel@emerson.com
 *          areaType: PRODUCT_INFO -> OEM
 * 09/07/29 lars.wetzel@emerson.com
 *          valid for READWRITE_IDR
 */

#include "AddArea.h"

using namespace ns_saHpiIdrAreaAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddArea::AddArea(char *line) : WriteIdrTestCase(line, WRITE_TC_READWRITE_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddArea::getName() {
    return "AddArea";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddArea::getDescription() {
    return "Add a new area.  Verify that it can be retrieved and\n"
           "that it's <i>AreaHeader.ReadOnly</i> field is False.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AddArea::getPrecondition() {
    return "Requires an Inventory for which an Area can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddArea::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddArea::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;

    SaErrorT error = saHpiIdrAreaAdd(sessionId,
                                     rptEntry->ResourceId, idrRec->IdrId, 
                                     SAHPI_IDR_AREATYPE_OEM, &areaId);

    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, IDR_AREA_ADD, SA_OK, error);
    } else {
         error = saHpiIdrAreaHeaderGet(sessionId,
                                       rptEntry->ResourceId, idrRec->IdrId,
                                       SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                       areaId, &nextAreaId, &header);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertFailure(TRACE, 
                    "The new AreaId could not be used to retrieve the new area.");
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET,
                            SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
        } else if (header.ReadOnly) {
            status.assertFailure(TRACE, "The Header.ReadOnly field is true, "
                                        "but it is always required to be false.");
        } else {
            status.assertPass();
        }

        error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId, idrRec->IdrId, areaId);
        status.checkError(TRACE, IDR_AREA_DELETE, error);
    }

    return status;
}


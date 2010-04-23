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
 *     Akmal A. Muqeeth <aax6@unh.edu>
 *
 * Changes
 * 09/06/25 lars.wetzel@emerson.com
 *          areaType: PRODUCT_INFO -> OEM
 *          delteArea: false -> true
 */

#include "DuplicateArea.h"

using namespace ns_saHpiIdrAreaAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DuplicateArea::DuplicateArea(char *line) : AddIdrTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DuplicateArea::getName() {
    return "DuplicateArea";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DuplicateArea::getDescription() {
    return "Check if aread represented by the areaId is already present in the IDR or not.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DuplicateArea::getPrecondition() {
    return "Requires an Inventory for that is not in read only mode and\n"
           "for which an Area can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DuplicateArea::getExpectedReturn() {
    return SA_ERR_HPI_DUPLICATE;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus DuplicateArea::runAddIdrTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiInventoryRecT *idrRec,
                                        SaHpiEntryIdT areaId) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrAreaAddById(sessionId,
                                     rptEntry->ResourceId,
                                     idrRec->IdrId,
                                     SAHPI_IDR_AREATYPE_OEM, areaId);
    if (error == SA_ERR_HPI_DUPLICATE) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_ADD_BY_ID,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

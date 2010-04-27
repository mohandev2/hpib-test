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
 * 09/07/29 lars.wetzel@emerson.com
 *          valid for READWRITE_IDR
 */

#include "InvalidAreaType.h"

using namespace ns_saHpiIdrAreaAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAreaType::InvalidAreaType(char *line) : WriteIdrTestCase(line, WRITE_TC_READWRITE_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAreaType::getName() {
    return "InvalidAreaType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAreaType::getDescription() {
    return "The AreaType is not one of the valid enumerated\n"
           "values for this type.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidAreaType::getPrecondition() {
    return "Requires an Inventory for which an Area can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAreaType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * Test a number of invalid area types.
 *****************************************************************************/

HpiTestStatus InvalidAreaType::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry,
                                               SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    int invalidAreaType[] = { (int) SAHPI_IDR_AREATYPE_PRODUCT_INFO + 1,
                              (int) SAHPI_IDR_AREATYPE_INTERNAL_USE - 1,
                              (int) SAHPI_IDR_AREATYPE_OEM - 1,
                              (int) SAHPI_IDR_AREATYPE_OEM + 1,
                              (int) SAHPI_IDR_AREATYPE_UNSPECIFIED - 1 };

    for (int i = 0; i < 5; i++) {
        SaErrorT error = saHpiIdrAreaAdd(sessionId,
                                         rptEntry->ResourceId,
                                         idrRec->IdrId, 
                                         (SaHpiIdrAreaTypeT) invalidAreaType[i],
                                         &areaId);

        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_AREA_ADD,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Invalid AreaType is 0x%X.", invalidAreaType[i]);
        } 
    }

    return status;
}


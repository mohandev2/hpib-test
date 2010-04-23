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
 *
 * 09/07/29 lars.wetzel@emerson.com
 *          valid for READWRITE_IDR
 */

#include "InvalidAreaId.h"

using namespace ns_saHpiIdrAreaAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAreaId::InvalidAreaId(char *line) : WriteIdrTestCase(line, 
							    WRITE_TC_READWRITE_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAreaId::getName() {
    return "InvalidAreaId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAreaId::getDescription() {
    return "The <i>AreaId</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidAreaId::getPrecondition() {
    return "Requires an Inventory for which an Area can be added by Id.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAreaId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidAreaId::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrAreaAddById(sessionId,
                                     rptEntry->ResourceId,
                                     idrRec->IdrId,
                                     SAHPI_IDR_AREATYPE_OEM, SAHPI_LAST_ENTRY);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_ADD_BY_ID,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


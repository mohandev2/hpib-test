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

#include "UnspecifiedAreaType.h"

using namespace ns_saHpiIdrAreaAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UnspecifiedAreaType::UnspecifiedAreaType(char *line) : WriteIdrTestCase(line, WRITE_TC_READWRITE_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UnspecifiedAreaType::getName() {
    return "UnspecifiedAreaType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UnspecifiedAreaType::getDescription() {
    return "Add an area with an <i>AreaType</i> of SAHPI_IR_AREATYPE_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *UnspecifiedAreaType::getPrecondition() {
    return "Requires an Inventory for which an Area can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UnspecifiedAreaType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus UnspecifiedAreaType::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry,
                                                   SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;

    SaErrorT error = saHpiIdrAreaAdd(sessionId,
                                     rptEntry->ResourceId, idrRec->IdrId, 
                                     SAHPI_IDR_AREATYPE_UNSPECIFIED, &areaId);

    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_ADD,
                             SA_ERR_HPI_INVALID_DATA, error);
    }

    return status;
}


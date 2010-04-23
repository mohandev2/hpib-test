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

#include "NullNextAreaId.h"

using namespace ns_saHpiIdrAreaHeaderGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullNextAreaId::NullNextAreaId(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullNextAreaId::getName() {
    return "NullNextAreaId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullNextAreaId::getDescription() {
    return "The <i>NextAreaId</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullNextAreaId::getPrecondition() {
    return "Requires an Inventory.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullNextAreaId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullNextAreaId::runIdrTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiIdrAreaHeaderT header;

    SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                           rptEntry->ResourceId,
                                           idrRec->IdrId,    
                                           SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                           SAHPI_FIRST_ENTRY, NULL, &header);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_HEADER_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


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

#include "NullHeader.h"

using namespace ns_saHpiIdrAreaHeaderGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullHeader::NullHeader(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullHeader::getName() {
    return "NullHeader";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullHeader::getDescription() {
    return "The <i>Header</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullHeader::getPrecondition() {
    return "Requires an Inventory.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullHeader::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullHeader::runIdrTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId;

    SaErrorT error = saHpiIdrAreaHeaderGet(sessionId, rptEntry->ResourceId,
                                           idrRec->IdrId,    
                                           SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                           SAHPI_FIRST_ENTRY,
                                           &nextAreaId, NULL);
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


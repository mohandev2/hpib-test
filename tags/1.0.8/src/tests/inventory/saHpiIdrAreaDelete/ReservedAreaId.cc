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

#include "ReservedAreaId.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

ReservedAreaId::ReservedAreaId(char *line) : WriteIdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReservedAreaId::getName() {
    return "ReservedAreaId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReservedAreaId::getDescription() {
    return "The <i>AreaId</i> is an invalid reserved value such as SAHPI_LAST_ENTRY.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReservedAreaId::getPrecondition() {
    return "Requires an Inventory that is not read-only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReservedAreaId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReservedAreaId::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;

    status.assertPass();

    SaErrorT error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId,
                                        idrRec->IdrId, SAHPI_LAST_ENTRY);
    if (error != SA_ERR_HPI_INVALID_PARAMS) {
        status.assertFailure(TRACE, IDR_AREA_DELETE,
                          SA_ERR_HPI_INVALID_PARAMS, error, 
                          "Using SAHPI_LAST_ENTRY for AreaId");
    }

    return status;
}


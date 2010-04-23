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
 * 09/07/30 anurag.saxena@emerson.com
 *          valid for WRITE_TC_ALL_IDR
 */

#include "ReservedAreaId.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReservedAreaId::ReservedAreaId(char *line) 
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
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
    return "The <i>AreaId</i> parameter is an invalid reserved\n"
           "value such as SAHPI_LAST_ENTRY.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReservedAreaId::getPrecondition() {
    return "Requires a read-only or read-write Inventory.";
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

    SaErrorT error = saHpiIdrFieldDelete(sessionId,
                                         rptEntry->ResourceId, idrRec->IdrId, 
                                         SAHPI_LAST_ENTRY, getValidIdrFieldId());
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_FIELD_DELETE,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

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

#include "AreaNotPresent.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AreaNotPresent::AreaNotPresent(char *line) : WriteIdrTestCase(line) {
}

/*****************************************************************************
 * Return then name of the test case.
 *****************************************************************************/

const char *AreaNotPresent::getName() {
    return "AreaNotPresent";
}

/*****************************************************************************
 * Return then description of the test case.
 *****************************************************************************/

const char *AreaNotPresent::getDescription() {
    return "The area identified by the <i>AreaId</i> parameter does\n"
           "not exist within the IDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AreaNotPresent::getPrecondition() {
    return "Requires a read-write Inventory.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AreaNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AreaNotPresent::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrFieldDelete(sessionId,
                                         rptEntry->ResourceId, idrRec->IdrId, 
                                         getInvalidIdrAreaId(), getValidIdrFieldId());
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_FIELD_DELETE,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

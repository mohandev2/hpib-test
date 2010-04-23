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
 * 09/07/28 lars.wetzel@emerson.com
 *          areaDelete is only possible with read-write IDR 
 */

#include "AreaIdNotPresent.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AreaIdNotPresent::AreaIdNotPresent(char *line) : WriteIdrTestCase(line, WRITE_TC_READWRITE_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AreaIdNotPresent::getName() {
    return "AreaIdNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AreaIdNotPresent::getDescription() {
    return "The area identified by the <i>AreaId</i> parameter does not\n"
           "exist within the IDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AreaIdNotPresent::getPrecondition() {
    return "Requires an Inventory for that is not in read only mode.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AreaIdNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AreaIdNotPresent::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                                SaHpiRptEntryT *rptEntry,
                                                SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId, 
                                        idrRec->IdrId, getInvalidIdrAreaId());
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_DELETE,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

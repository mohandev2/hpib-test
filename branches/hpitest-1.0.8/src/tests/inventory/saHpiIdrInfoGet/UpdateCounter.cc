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

#include "UpdateCounter.h"

using namespace ns_saHpiIdrInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UpdateCounter::UpdateCounter(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UpdateCounter::getName() {
    return "UpdateCounter";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UpdateCounter::getDescription() {
    return "The update counter is incremented whenever an addition is made "
           "to the IDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *UpdateCounter::getPrecondition() {
    return "Requires a read-write Inventory.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UpdateCounter::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Check the UpdateCount; add a new area; recheck the UpdateCount; it must
 * have been incremented.
 *****************************************************************************/

HpiTestStatus UpdateCounter::runIdrTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiIdrInfoT idrInfoBefore;
    SaHpiIdrInfoT idrInfoAfter;
    SaHpiIdrIdT idrId = idrRec->IdrId;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    // Get the current value of the UpdateCount.

    SaErrorT error = saHpiIdrInfoGet(sessionId, resourceId, idrId, &idrInfoBefore);
    if (error != SA_OK) {
        status.assertError(TRACE, IDR_INFO_GET, SA_OK, error);
    } else if (idrInfoBefore.ReadOnly) {
        status.assertNotSupport();
    } else {

        // Add an area which should cause the UpdateCount to increment.

        error = saHpiIdrAreaAdd(sessionId, resourceId, idrId,
                                SAHPI_IDR_AREATYPE_PRODUCT_INFO, &areaId);

        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_ADD, SA_OK, error);
        } else {

            // Check the UpdateCount.
            error = saHpiIdrInfoGet(sessionId, resourceId, idrId, &idrInfoAfter);

            if (error != SA_OK) {
                status.assertError(TRACE, IDR_INFO_GET, SA_OK, error);
            } else if (idrInfoAfter.UpdateCount > idrInfoBefore.UpdateCount) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                        "After UpdateCount (%d) is <= Before UpdateCount (%d).",
                        idrInfoAfter.UpdateCount, idrInfoBefore.UpdateCount);
            }

            // Remove the Area that we added.

            error = saHpiIdrAreaDelete(sessionId, resourceId, idrId, areaId);
            status.checkError(TRACE, IDR_AREA_DELETE, error);
        }
    }

    return status;
}


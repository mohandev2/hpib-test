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

#include "ReadOnlyIdr.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnlyIdr::ReadOnlyIdr(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnlyIdr::getName() {
    return "ReadOnlyIdr";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnlyIdr::getDescription() {
    return "The IDR is read-only as deletions are not permitted "
           "for an area from a read-only IDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnlyIdr::getPrecondition() {
    return "Requires a read-only Inventory with at least one Area.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnlyIdr::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *
 * Get the first Area from a Read-only IDR and try to delete it.
 *****************************************************************************/

HpiTestStatus ReadOnlyIdr::runIdrTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiIdrInfoT idrInfo;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;

    SaErrorT error = saHpiIdrInfoGet(sessionId, rptEntry->ResourceId,
                                     idrRec->IdrId, &idrInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, IDR_INFO_GET, SA_OK, error);
    } else if (!idrInfo.ReadOnly) {
        status.assertNotSupport();
    } else {
        error = saHpiIdrAreaHeaderGet(sessionId, rptEntry->ResourceId,
                                      idrRec->IdrId,
                                      SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                      SAHPI_FIRST_ENTRY, 
                                      &nextAreaId, &header);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
        } else {
            error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId,
                                       idrRec->IdrId, header.AreaId);
            if (error == SA_ERR_HPI_READ_ONLY) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, IDR_AREA_DELETE,
                                     SA_ERR_HPI_READ_ONLY, error);
            }
        }
    }

    return status;
}

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
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldAdd;

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
    return "The IDR identified by <i>IdrId</i>, is read-only, and does\n"
           "not allow modification to its fields.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnlyIdr::getPrecondition() {
    return "Requires a read-only Inventory with a read-only Area for which\n"
           "free space is available and the new field is not rejected by the\n"
           "implementation, i.e. SA_ERR_HPI_INVALID_DATA.";
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
 * If this is a read-only IDR, try adding a field to each area in that IDR.
 *****************************************************************************/

HpiTestStatus ReadOnlyIdr::runIdrTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;
    SaHpiIdrInfoT idrInfo;

    SaErrorT error = saHpiIdrInfoGet(sessionId, rptEntry->ResourceId,
                                     idrRec->IdrId, &idrInfo);
    if (error != SA_OK) {
        status.assertError(TRACE, IDR_INFO_GET, SA_OK, error);
    } else if (!idrInfo.ReadOnly) {
        status.assertNotSupport();
    } else {
        status.assertNotSupport();
        while (nextAreaId != SAHPI_LAST_ENTRY && !status.hasFault()) {
            areaId = nextAreaId;
            error = saHpiIdrAreaHeaderGet(sessionId,
                                          rptEntry->ResourceId,
                                          idrRec->IdrId,
                                          SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                          areaId, &nextAreaId, &header);

            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, IDR_AREA_HEADER_GET,
                                   SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
            } else {
                status.add(TRACE, addField(sessionId, rptEntry->ResourceId,
                                           idrRec->IdrId, header.AreaId));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Try to add a field to an area that is in a read-only IDR.
 *****************************************************************************/

HpiTestStatus ReadOnlyIdr::addField(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId, 
                                    SaHpiIdrIdT idrId,
                                    SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;

    InventoryHelper::fill(&field, areaId);
    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, &field);

    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_READ_ONLY) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_FIELD_ADD, SA_ERR_HPI_READ_ONLY, error);
    }

    return status;
}


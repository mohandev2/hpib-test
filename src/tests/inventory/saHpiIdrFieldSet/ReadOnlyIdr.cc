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

using namespace ns_saHpiIdrFieldSet;

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
    return "The IDR is read-only and does not allow modification to its fields.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnlyIdr::getPrecondition() {
    return "Requires a read-only Inventory with at least one Field.";
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
 * For each area in a read-only IDR, try setting a field.
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
                                          areaId,
                                          &nextAreaId, &header);

            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, IDR_AREA_HEADER_GET,
                                   SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
            } else {
                status.add(TRACE, setField(sessionId, rptEntry->ResourceId,
                                           idrRec->IdrId, header.AreaId));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * In a read-only IDR, do a pretend set to a field (not really changing
 * anything).
 *****************************************************************************/

HpiTestStatus ReadOnlyIdr::setField(SaHpiSessionIdT sessionId,
                                    SaHpiResourceIdT resourceId, 
                                    SaHpiIdrIdT idrId,
                                    SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;
    SaHpiEntryIdT nextFieldId;

    SaErrorT error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
                                      SAHPI_IDR_FIELDTYPE_UNSPECIFIED,
                                      SAHPI_FIRST_ENTRY,
                                      &nextFieldId, &field);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
    } else {
        error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &field);
        if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_FIELD_SET,
                                 SA_ERR_HPI_READ_ONLY, error);
        }
    }

    return status;
}


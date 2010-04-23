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

#include "ReadOnlyArea.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnlyArea::ReadOnlyArea(char *line) : WriteIdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnlyArea::getName() {
    return "ReadOnlyArea";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnlyArea::getDescription() {
    return "The IDA is read-only and therefore cannot be deleted.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnlyArea::getPrecondition() {
    return "Requires an Inventory for that is not in read-only mode as well\n"
           "as an Area that is read-only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnlyArea::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *
 * Find a read-only area and then try to delete it.
 *****************************************************************************/

HpiTestStatus ReadOnlyArea::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    bool found;

    status.add(TRACE, findReadOnlyArea(sessionId, rptEntry->ResourceId,
                                       idrRec->IdrId, &areaId, &found));
    if (status.isOkay()) {
        if (!found) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId,
                                                idrRec->IdrId, areaId);
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

/*****************************************************************************
 * Find a read-only area if there is one.
 *****************************************************************************/

HpiTestStatus ReadOnlyArea::findReadOnlyArea(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId, 
                                             SaHpiIdrIdT idrId,
                                             SaHpiEntryIdT *areaId,
                                             bool *found) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;

    *found = false;
    while (nextAreaId != SAHPI_LAST_ENTRY) {
        *areaId = nextAreaId;
        SaErrorT error = saHpiIdrAreaHeaderGet(sessionId, resourceId, idrId,
                                              SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                              *areaId, &nextAreaId, &header);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
            break;
        } else if (header.ReadOnly) {
            *areaId = header.AreaId;
            *found = true;
            break;
        }
    }

    return status;
}


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

#include "AreaTypeMismatch.h"

using namespace ns_saHpiIdrAreaHeaderGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AreaTypeMismatch::AreaTypeMismatch(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AreaTypeMismatch::getName() {
    return "AreaTypeMismatch";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AreaTypeMismatch::getDescription() {
    return "The <i>AreaType</i> parameter is set to a specific area type,\n"
           "but an area matching both the <i>AreaId</i> parameter and\n"
           "<i>AreaType</i> parameter does not exist.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AreaTypeMismatch::getPrecondition() {
    return "Requires an Inventory for which either CHASSIS, BOARD, or PRODUCT\n"
           "isn't being used by any of its Areas.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AreaTypeMismatch::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *
 *
 * If we can find an AreaType that isn't being used in the IDR, then query
 * for the first entry with that has that AreaType.  Of course, this should
 * fail.
 *****************************************************************************/

HpiTestStatus AreaTypeMismatch::runIdrTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaType;
    SaHpiIdrAreaHeaderT header;
    SaHpiIdrAreaTypeT areaType;
    bool foundUnusedArea;

    status.add(TRACE, findUnusedAreaType(sessionId, rptEntry->ResourceId, 
                                         idrRec->IdrId,
                                         &areaType, &foundUnusedArea));
    if (status.isOkay()) {
        if (!foundUnusedArea) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                                   rptEntry->ResourceId, 
                                                   idrRec->IdrId, areaType,
                                                   SAHPI_FIRST_ENTRY,
                                                   &nextAreaType, &header);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, IDR_AREA_HEADER_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Find an Area Type that isn't being used by any of the areas in this IDR.
 *****************************************************************************/

HpiTestStatus AreaTypeMismatch::findUnusedAreaType(SaHpiSessionIdT     sessionId,
                                                   SaHpiResourceIdT    resourceId,
                                                   SaHpiIdrIdT         idrId,
                                                   SaHpiIdrAreaTypeT   *areaType,
                                                   bool                *foundUnusedArea) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId, areaId;
    SaHpiIdrAreaHeaderT header;
    bool found[3];

    *foundUnusedArea = false;
    for (int i = 0; i < 3; i++) {
        found[i] = false;
    }

    // Go through the IDR.  For each area type we find,
    // set a mark to indicate that we found it.

    nextAreaId = SAHPI_FIRST_ENTRY;
    while ((nextAreaId != SAHPI_LAST_ENTRY) && (status.isOkay())) {

        areaId = nextAreaId;

        SaErrorT error = saHpiIdrAreaHeaderGet(sessionId, resourceId, idrId,
                                               SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                               areaId, &nextAreaId, &header);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
        } else if (header.Type == SAHPI_IDR_AREATYPE_CHASSIS_INFO) {
            found[0] = true;
        } else if (header.Type == SAHPI_IDR_AREATYPE_BOARD_INFO) {
            found[1] = true;
        } else if (header.Type == SAHPI_IDR_AREATYPE_PRODUCT_INFO) {
            found[2] = true;
        }
    }

    if (status.isOkay()) {
        // Check for an AreaType that wasn't found.
        if (!found[0]) {
            *foundUnusedArea = true;
            *areaType = SAHPI_IDR_AREATYPE_CHASSIS_INFO;
        } else if (!found[1]) {
            *foundUnusedArea = true;
            *areaType = SAHPI_IDR_AREATYPE_BOARD_INFO;
        } else if (!found[2]) {
            *foundUnusedArea = true;
            *areaType = SAHPI_IDR_AREATYPE_PRODUCT_INFO;
        }
    }

    return status;
}

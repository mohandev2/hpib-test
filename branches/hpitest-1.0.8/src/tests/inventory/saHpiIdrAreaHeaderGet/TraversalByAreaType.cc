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

#include "TraversalByAreaType.h"

using namespace ns_saHpiIdrAreaHeaderGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TraversalByAreaType::TraversalByAreaType(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TraversalByAreaType::getName() {
    return "TraversalByAreaType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TraversalByAreaType::getDescription() {
    return "Traverse all the areas for a specific <i>AreaType</i>.  Try all\n"
           "four valid AreaTypes: CHASSIS_INFO, BOARD_INFO, PRODUCT_INFO,\n"
           "and OEM.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *TraversalByAreaType::getPrecondition() {
    return "Requires an Inventory with at least one Area.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT TraversalByAreaType::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus TraversalByAreaType::runIdrTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;
    SaHpiIdrAreaTypeT areaType[] = { SAHPI_IDR_AREATYPE_CHASSIS_INFO,
                                     SAHPI_IDR_AREATYPE_BOARD_INFO,
                                     SAHPI_IDR_AREATYPE_PRODUCT_INFO,
                                     SAHPI_IDR_AREATYPE_OEM };

    status.assertNotSupport();
    for (int i = 0; i < 4; i++) {
        nextAreaId = SAHPI_FIRST_ENTRY;
        while (nextAreaId != SAHPI_LAST_ENTRY) {
            areaId = nextAreaId;
            SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                                   rptEntry->ResourceId,
                                                   idrRec->IdrId,    
                                                   areaType[i], areaId,
                                                   &nextAreaId, &header);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertFailure(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
                break;
            } else if (header.Type != areaType[i]) {
                status.assertFailure(TRACE, 
                            "Header.Type did not match AreaType of %s.", 
                            HpiString::idrAreaType(areaType[i]));
                break;
            } else {
                status.assertPass();
            }
        }
    }

    return status;
}


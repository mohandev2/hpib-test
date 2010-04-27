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

using namespace ns_saHpiIdrAreaHeaderGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AreaNotPresent::AreaNotPresent(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AreaNotPresent::getName() {
    return "AreaNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AreaNotPresent::getDescription() {
    return "The <i>AreaType</i> parameter is set to\n"
           "SAHPI_IDR_AREATYPE_UNSPECIFIED, and the area specified\n"
           "by the <i>AreaId</i> parameter does not exist in the IDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AreaNotPresent::getPrecondition() {
    return "Requires an Inventory.";
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

HpiTestStatus AreaNotPresent::runIdrTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;

    SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                           rptEntry->ResourceId, 
                                           idrRec->IdrId,
                                           SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                           getInvalidIdrAreaId(),
                                           &nextAreaId, &header);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_HEADER_GET,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

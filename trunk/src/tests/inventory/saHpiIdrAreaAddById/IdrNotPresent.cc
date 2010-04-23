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
 *     Akmal A. Muqeeth <aax6@unh.edu>
 *
 * Changes
 * 09/06/25 lars.wetzel@emerson.com
 *          areaType: PRODUCT_INFO -> OEM
 */

#include "IdrNotPresent.h"

using namespace ns_saHpiIdrAreaAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IdrNotPresent::IdrNotPresent(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IdrNotPresent::getName() {
    return "IdrNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IdrNotPresent::getDescription() {
    return "The IDR is not present.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *IdrNotPresent::getPrecondition() {
    return "Requires a resource that supports Inventories.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IdrNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus IdrNotPresent::runResourceTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId = SAHPI_FIRST_ENTRY;

    if (!hasInventoryCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiIdrAreaAddById(sessionId, rptEntry->ResourceId, 
                                         getInvalidIdrId(), 
                                         SAHPI_IDR_AREATYPE_OEM,
                                         areaId);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_AREA_ADD_BY_ID,
                                 SA_ERR_HPI_NOT_PRESENT, error);
        }
    }

    return status;
}

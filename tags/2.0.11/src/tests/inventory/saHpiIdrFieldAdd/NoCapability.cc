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

#include "NoCapability.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoCapability::NoCapability(char *line)
: HpiNoCapabilityTestCase(line, "Inventory") {
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoCapability::runResourceTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;

    if (hasInventoryCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        InventoryHelper::fill(&field, getValidIdrAreaId());
        SaErrorT error = saHpiIdrFieldAdd(sessionId, rptEntry->ResourceId, 
                                          SAHPI_DEFAULT_INVENTORY_ID, &field);

        if (error == SA_ERR_HPI_CAPABILITY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_FIELD_ADD,
                                 SA_ERR_HPI_CAPABILITY, error);
        }
    }

    return status;
}

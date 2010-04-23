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

#include "InvalidResource.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidResource::InvalidResource(char *line) : HpiInvalidResourceTestCase(line) {
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidResource::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;

    InventoryHelper::fill(&field, getValidIdrAreaId(), getValidIdrFieldId());
    SaErrorT error = saHpiIdrFieldSet(sessionId, getInvalidResourceId(),
                                      SAHPI_DEFAULT_INVENTORY_ID, &field);

    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_INVALID_RESOURCE) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_FIELD_SET, 
                             SA_ERR_HPI_INVALID_RESOURCE, error);
    } 

    return status;
}


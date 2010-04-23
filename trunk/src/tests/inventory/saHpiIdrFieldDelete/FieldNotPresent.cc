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
 *
 * Changes
 * 09/07/30 anurag.saxena@emerson.com
 *          valid for WRITE_TC_ALL_IDR
 */

#include "FieldNotPresent.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FieldNotPresent::FieldNotPresent(char *line) 
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *FieldNotPresent::getName() {
    return "FieldNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *FieldNotPresent::getDescription() {
    return "A field identified by the <i>FieldId</i> parameter\n"
           "does not exist with the IDA.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *FieldNotPresent::getPrecondition() {
    return "Requires a read-only or read-write Inventory with a read-write\n"
           "Area.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT FieldNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, try deleting a non-existing field.
 *****************************************************************************/

HpiTestStatus FieldNotPresent::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry,
                                               SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;
    SaHpiEntryIdT nextAreaId = SAHPI_FIRST_ENTRY;
    SaHpiIdrAreaHeaderT header;

    status.assertNotSupport();
    while (nextAreaId != SAHPI_LAST_ENTRY && !status.hasFault()) {
        areaId = nextAreaId;
        SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                               rptEntry->ResourceId,
                                               idrRec->IdrId,
                                               SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                               areaId, &nextAreaId, &header);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK,
                               SA_ERR_HPI_NOT_PRESENT, error);
        } else if (!header.ReadOnly) {
            status.add(TRACE, deleteField(sessionId, rptEntry->ResourceId,
                                          idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * Try to delete a field from an area using an invalid field id.
 *****************************************************************************/

HpiTestStatus FieldNotPresent::deleteField(SaHpiSessionIdT sessionId,
                                           SaHpiResourceIdT resourceId,
                                           SaHpiIdrIdT idrId,
                                           SaHpiEntryIdT areaId) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrFieldDelete(sessionId, resourceId,
                                         idrId, areaId,
                                         getInvalidIdrFieldId());
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_FIELD_DELETE,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

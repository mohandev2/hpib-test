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

#include "FieldNotPresent.h"

using namespace ns_saHpiIdrFieldGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FieldNotPresent::FieldNotPresent(char *line) : IdrTestCase(line) {
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
    return "The <i>FieldType</i> parameter is set to\n"
           "SAHPI_IDR_FIELDTYPE_UNSPECIFIED, and the field\n"
           "specified by the <i>FieldId</i> parameter does not\n"
           "exist in the IDA.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *FieldNotPresent::getPrecondition() {
    return "Requires a read-write Inventory with at least one Area.";
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
 * Get the first Area in the IDR and use it's AreaId to query for the
 * field using an invalid FieldId.
 *****************************************************************************/

HpiTestStatus FieldNotPresent::runIdrTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT nextAreaId;
    SaHpiIdrAreaHeaderT header;
    SaHpiEntryIdT nextFieldId;
    SaHpiIdrFieldT field;

    SaErrorT error = saHpiIdrAreaHeaderGet(sessionId,
                                           rptEntry->ResourceId,
                                           idrRec->IdrId,
                                           SAHPI_IDR_AREATYPE_UNSPECIFIED,
                                           SAHPI_FIRST_ENTRY, 
                                           &nextAreaId, &header);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_AREA_HEADER_GET, SA_OK, error);
    } else {
        error = saHpiIdrFieldGet(sessionId, rptEntry->ResourceId, idrRec->IdrId, 
                                 header.AreaId, SAHPI_IDR_FIELDTYPE_UNSPECIFIED,
                                 getInvalidIdrFieldId(), &nextFieldId, &field);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, IDR_FIELD_GET,
                                 SA_ERR_HPI_NOT_PRESENT, error);
        }
    }

    return status;
}

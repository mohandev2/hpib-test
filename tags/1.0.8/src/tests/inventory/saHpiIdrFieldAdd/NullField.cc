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

#include "NullField.h"

using namespace ns_saHpiIdrFieldAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullField::NullField(char *line) : WriteIdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullField::getName() {
    return "NullField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullField::getDescription() {
    return "The <i>Field</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullField::getPrecondition() {
    return "Requires a read-write Inventory with a read-write Area for which\n"
           "free space is available.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullField::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, try adding a field using a NULL Field parameter.
 *****************************************************************************/

HpiTestStatus NullField::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
            status.assertError(TRACE, IDR_AREA_HEADER_GET,
                               SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
        } else if (!header.ReadOnly) {
            status.add(TRACE, addField(sessionId,
                                       rptEntry->ResourceId, idrRec->IdrId));
        }
    }

    return status;
}

/*****************************************************************************
 * Try adding a field using a NULL Field parameter.
 *****************************************************************************/

HpiTestStatus NullField::addField(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId, 
                                  SaHpiIdrIdT idrId) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, NULL);

    if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_ERR_HPI_INVALID_PARAMS) {
        status.assertFailure(TRACE, IDR_FIELD_ADD, SA_ERR_HPI_INVALID_PARAMS, error);
    } else {
        status.assertPass();
    }

    return status;
}
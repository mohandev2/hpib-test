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
 * 09/07/30 anurag.saxena@emerson.com
 *          valid for WRITE_TC_ALL_IDR
 */

#include "UnspecifiedFieldType.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UnspecifiedFieldType::UnspecifiedFieldType(char *line)
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UnspecifiedFieldType::getName() {
    return "UnspecifiedFieldType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UnspecifiedFieldType::getDescription() {
    return "The field type, <i>Field->Type</i> cannot be "
           "SAHPI_IDR_FIELDTYPE_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *UnspecifiedFieldType::getPrecondition() {
    return "Requires a read-only or read-write Inventory with a read-write\n"
           "Area for which free space is available and the new field is not\n"
           "rejected by the implementation, i.e. SA_ERR_HPI_INVALID_DATA.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UnspecifiedFieldType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, try adding a field using a field type of UNSPECIFIED.
 *****************************************************************************/

HpiTestStatus UnspecifiedFieldType::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
            status.add(TRACE, addField(sessionId, rptEntry->ResourceId,
                                       idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * Try to add a field to an area using a field type of UNSPECIFIED.
 *****************************************************************************/

HpiTestStatus UnspecifiedFieldType::addField(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId, 
                                             SaHpiIdrIdT idrId,
                                             SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;

    InventoryHelper::fill(&field, areaId,SAHPI_FIRST_ENTRY);
    field.Type = SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
    SaErrorT error = saHpiIdrFieldAddById(sessionId, resourceId, idrId, &field);

    if (error == SA_ERR_HPI_INVALID_DATA) {
       //see MANTIS IR 234 
       //INVALID DATA is also allowed 
       status.assertPass();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_ERR_HPI_INVALID_PARAMS) {
        status.assertFailure(TRACE, IDR_FIELD_ADD_BY_ID,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } else {
        status.assertPass();
    }

    return status;
}

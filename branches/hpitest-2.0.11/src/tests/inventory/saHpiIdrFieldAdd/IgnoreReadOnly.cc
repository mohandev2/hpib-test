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

#include "IgnoreReadOnly.h"
#include "InventoryHelper.h"

using namespace ns_saHpiIdrFieldAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IgnoreReadOnly::IgnoreReadOnly(char *line) 
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IgnoreReadOnly::getName() {
    return "IgnoreReadOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IgnoreReadOnly::getDescription() {
    return "Addition of a read-only Inventory Data Field is not allowed;\n"
           "therefore the <i>ReadOnly</i> element in the <i>Field</i>\n"
           "parameter is ignored.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *IgnoreReadOnly::getPrecondition() {
    return "Requires a read-only or read-write Inventory with a read-write\n"
           "Area for which free space is available and the new field is not\n"
           "rejected by the implementation, i.e. SA_ERR_HPI_INVALID_DATA.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IgnoreReadOnly::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * For each Area that we can add a field to, add a field with ReadOnly set
 * to TRUE.
 *****************************************************************************/

HpiTestStatus IgnoreReadOnly::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
 * Add a field to the given area with a ReadOnly field of TRUE which should
 * be ignored.  After adding the field, get it and verify that the ReadOnly
 * field is FALSE.  
 *****************************************************************************/

HpiTestStatus IgnoreReadOnly::addField(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId,
                                       SaHpiIdrIdT idrId,
                                       SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;
    SaHpiIdrFieldT newField;
    SaHpiEntryIdT nextFieldId;

    InventoryHelper::fill(&field, areaId);
    field.ReadOnly = SAHPI_TRUE;

    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, &field);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, IDR_FIELD_ADD, SA_OK, error);
    } else {
        error = saHpiIdrFieldGet(sessionId, resourceId, idrId, areaId,
                                 SAHPI_IDR_FIELDTYPE_UNSPECIFIED, field.FieldId, 
                                 &nextFieldId, &newField);
        if (error != SA_OK) {
            status.assertError(TRACE, IDR_FIELD_GET, SA_OK, error);
        } else if (newField.ReadOnly) {
            status.assertFailure(TRACE, "The ReadOnly field was incorrectly set to TRUE.");
        } else {
            status.assertPass();
        }

        error = saHpiIdrFieldDelete(sessionId, resourceId, idrId, areaId, field.FieldId);
        status.checkError(TRACE, IDR_FIELD_DELETE, error);
    }

    return status;
}

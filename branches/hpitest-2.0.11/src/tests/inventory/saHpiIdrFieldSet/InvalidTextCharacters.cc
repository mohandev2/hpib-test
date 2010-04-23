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

#include "InvalidTextCharacters.h"
#include "InventoryHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidTextCharacters::InvalidTextCharacters(char *line)
: WriteIdrTestCase(line, WRITE_TC_ALL_IDR) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getName() {
    return "InvalidTextCharacters";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getDescription() {
    return "The <i>Data</i> field contains characters that are not\n"
           "legal according to the value of <i>DataType</i>.  The tested\n"
           "DataTypes are SAHPI_TL_TYPE_UNICODE, SAHPI_TL_TYPE_BCDPLUS,\n"
           "and SAHPI_TL_TYPE_ASCII6.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getPrecondition() {
    return "Requires a read-only or read-write Inventory with a read-write\n"
           "Area for which free space is available and a new field for\n"
           "testing purposes is not rejected by the implementation,\n"
           "i.e. SA_ERR_HPI_INVALID_DATA.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidTextCharacters::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * For each area, try to change a field's Data to invalid text characters.
 *****************************************************************************/

HpiTestStatus InvalidTextCharacters::runWriteIdrTest(SaHpiSessionIdT sessionId,
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
                                               areaId,
                                               &nextAreaId, &header);

        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, IDR_AREA_HEADER_GET,
                               SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
        } else if (!header.ReadOnly) {
            status.add(TRACE, setField(sessionId, rptEntry->ResourceId,
                                       idrRec->IdrId, header.AreaId));
        }
    }

    return status;
}

/*****************************************************************************
 * Add a field to an area and then try to set's Data to a set of invalid
 * characters.  Test unicode, bcdplus, and ascii6.
 *****************************************************************************/

HpiTestStatus InvalidTextCharacters::setField(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId, 
                                              SaHpiIdrIdT idrId,
                                              SaHpiEntryIdT areaId) {
    HpiTestStatus status;
    SaHpiIdrFieldT field;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE, 
                                  SAHPI_TL_TYPE_BCDPLUS,
                                  SAHPI_TL_TYPE_ASCII6 };

    InventoryHelper::fill(&field, areaId);
    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, &field);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_FIELD_ADD, SA_OK, error);
    } else {
        for (int i = 0; i < 3; i++) {
            TextBufferHelper::fillInvalidData(&field.Field, dataType[i]);

            error = saHpiIdrFieldSet(sessionId, resourceId, idrId, &field);
            if (error == SA_ERR_HPI_INVALID_DATA) {
                status.assertNotSupport();
            } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
                status.assertNotSupport();
            } else if (error != SA_ERR_HPI_INVALID_PARAMS) {
                status.assertFailure(TRACE, IDR_FIELD_SET,
                                     SA_ERR_HPI_INVALID_PARAMS, error);
            } else {
                status.assertPass();
            }
        }
        error = saHpiIdrFieldDelete(sessionId, resourceId,
                                    idrId, areaId, field.FieldId);
        status.checkError(TRACE, IDR_FIELD_DELETE, error);
    }

    return status;
}

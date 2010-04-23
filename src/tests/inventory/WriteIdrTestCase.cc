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

#include "WriteIdrTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WriteIdrTestCase::WriteIdrTestCase() : IdrTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WriteIdrTestCase::WriteIdrTestCase(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Before running the main test, we must verify that we can actually
 * write to the IDR.
 *****************************************************************************/

HpiTestStatus WriteIdrTestCase::runIdrTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiIdrInfoT idrInfo;

    SaErrorT error = saHpiIdrInfoGet(sessionId, rptEntry->ResourceId,
                                     idrRec->IdrId, &idrInfo);

    if (error != SA_OK) {
        status.assertError(TRACE, IDR_INFO_GET, SA_OK, error);
    } else if (idrInfo.ReadOnly) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, runWriteIdrTest(sessionId, rptEntry, idrRec));
    }

    return status;
}

/*****************************************************************************
 * Adds an IDR Field to an Area.
 *****************************************************************************/

HpiTestStatus WriteIdrTestCase::addField(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT resourceId, 
                                         SaHpiIdrIdT idrId,
                                         SaHpiEntryIdT areaId,
                                         SaHpiIdrFieldT *field) {
    HpiTestStatus status;

    field->AreaId = areaId;
    field->FieldId = 0;
    field->ReadOnly = SAHPI_FALSE;
    field->Type = SAHPI_IDR_FIELDTYPE_ASSET_TAG;
    field->Field.DataLength = 1;
    field->Field.Data[0] = 'a';
    field->Field.DataType = SAHPI_TL_TYPE_TEXT;
    field->Field.Language = SAHPI_LANG_ENGLISH;

    SaErrorT error = saHpiIdrFieldAdd(sessionId, resourceId, idrId, field);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_FIELD_ADD, SA_OK, error);
    } 
    
    return status;
}


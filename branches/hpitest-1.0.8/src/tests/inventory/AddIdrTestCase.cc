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

#include "AddIdrTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddIdrTestCase::AddIdrTestCase(bool delArea) : WriteIdrTestCase() {
    this->deleteArea = delArea;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddIdrTestCase::AddIdrTestCase(char *line, bool delArea) : WriteIdrTestCase(line) {
    this->deleteArea = delArea;
}

/*****************************************************************************
 * Before running the main test, add an Area to the IDR and provide that
 * new AreaId to the main test.  The "deleteArea" field indicates whether
 * or not the delete the area after the test has completed, i.e. sometimes
 * the main test will delete the area.
 *****************************************************************************/

HpiTestStatus AddIdrTestCase::runWriteIdrTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiEntryIdT areaId;

    SaErrorT error = saHpiIdrAreaAdd(sessionId, rptEntry->ResourceId, idrRec->IdrId, 
                                     SAHPI_IDR_AREATYPE_PRODUCT_INFO, &areaId);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, IDR_AREA_ADD, SA_OK, error);
    } else {
        status.add(TRACE, runAddIdrTest(sessionId, rptEntry, idrRec, areaId));

        if (deleteArea) {
            error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId, idrRec->IdrId, areaId);
            status.checkError(TRACE, IDR_AREA_DELETE, error);
        }
    }

    return status;
}

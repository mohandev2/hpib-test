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

#include "DeleteArea.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DeleteArea::DeleteArea(char *line) : AddIdrTestCase(line, false) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DeleteArea::getName() {
    return "DeleteArea";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DeleteArea::getDescription() {
    return "Delete an area.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DeleteArea::getPrecondition() {
    return "Requires an Inventory for that is not in read only mode and\n"
           "for which an Area can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DeleteArea::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus DeleteArea::runAddIdrTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiInventoryRecT *idrRec,
                                        SaHpiEntryIdT areaId) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrAreaDelete(sessionId, rptEntry->ResourceId,
                                        idrRec->IdrId, areaId);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_AREA_DELETE, SA_OK, error);
    }

    return status;
}

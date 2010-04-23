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

#include "TestStatus.h"
#include "HpiResourceTestCase.h"
#include "LocationMgr.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiResourceTestCase::HpiResourceTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiResourceTestCase::HpiResourceTestCase(char *line) : HpiSessionTestCase(line) { 
}

/*****************************************************************************
 * HPI Resource Test Case.
 *****************************************************************************/

HpiTestStatus HpiResourceTestCase::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    status.assertNotSupport();
    SaHpiRptEntryT rptEntry;
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId,
                                          &nextEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else {
            LocationMgr::push("Resource", rptEntry.ResourceId);
            status.merge(runResourceTest(sessionId, &rptEntry));
            LocationMgr::pop();
        }
    }

    return status;
}

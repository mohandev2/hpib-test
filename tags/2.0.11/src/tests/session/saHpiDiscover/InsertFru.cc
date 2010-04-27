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

#include "InsertFru.h"

using namespace ns_saHpiDiscover;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InsertFru::InsertFru(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InsertFru::getName() {
    return "InsertFru";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InsertFru::getDescription() {
    return ".";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InsertFru::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * This is a manual test.
 *****************************************************************************/

bool InsertFru::isManual() {
    return true;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InsertFru::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    int beforeCount;
    int afterCount;

    SaErrorT error = saHpiDiscover(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SESSION_DISCOVER, SA_OK, error);
    } else {
        status.add(TRACE, getResourceCount(sessionId, &beforeCount));
        if (status.isOkay()) {
            promptInsertFruNoWait(sessionId);

            error = saHpiDiscover(sessionId);
            if (error != SA_OK) {
                status.assertError(TRACE, SESSION_DISCOVER, SA_OK, error);
            } else {
                status.add(TRACE, getResourceCount(sessionId, &afterCount));
                if (status.isOkay()) {
                    if (beforeCount + 1 == afterCount) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, 
                                "Failed to find one extra resource entry in the RPT.");
                    }
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Count the number of resources in the RPT.
 *****************************************************************************/

HpiTestStatus InsertFru::getResourceCount(SaHpiSessionIdT sessionId,
                                          int *count) {
    HpiTestStatus status;

    *count = 0;
    SaHpiRptEntryT rptEntry;
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId,
                                          &nextEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error == SA_OK) {
            (*count)++;
        } else {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        }
    }

    return status;
}

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

#include "GetAll.h"

using namespace ns_saHpiDrtEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAll::GetAll(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAll::getName() {
    return "GetAll";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAll::getDescription() {
    return "Retrieve all of the DRT entries.";
}

/*****************************************************************************
 * Return the precondition for the test case.
 *****************************************************************************/

const char *GetAll::getPrecondition() {
    return "Requires one or more entries in the DRT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAll::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetAll::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDrtEntryT drtEntry;

    status.assertNotSupport();
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiDrtEntryGet(sessionId, entryId, &nextEntryId, &drtEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            if (entryId != SAHPI_FIRST_ENTRY) {
                status.assertFailure(TRACE, "saHpiDrtEntryGet() returned "
                                            "SA_ERR_HPI_NOT_PRESENT but " 
                                            "EntryId was not SAHPI_FIRST_ENTRY.");
            }
            break;
        } else if (error == SA_OK) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, DRT_ENTRY_GET, SA_OK, error);
            break;
        }
    }

    return status;
}


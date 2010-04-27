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

using namespace ns_saHpiRptEntryGet;

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
    return "Retrieve all of the resource entries from the RPT table.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetAll::getPrecondition() {
    return "Must have one or more entries in the RPT table.";
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
    SaHpiRptEntryT rptEntry;

    status.assertNotSupport();
    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (nextEntryId != SAHPI_LAST_ENTRY) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId,
                                          &nextEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            if (entryId != SAHPI_FIRST_ENTRY) {
                status.assertFailure(TRACE, "saHpiRptEntryGet() returned "
                                            "SA_ERR_HPI_NOT_PRESENT but "
                                            "EntryId was not SAHPI_FIRST_ENTRY.");
            }
            break;
        } else if (error == SA_OK) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        }
    } 

    return status;
}


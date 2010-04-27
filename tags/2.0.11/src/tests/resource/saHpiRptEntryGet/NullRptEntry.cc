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

#include "NullRptEntry.h"

using namespace ns_saHpiRptEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullRptEntry::NullRptEntry(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullRptEntry::getName() {
    return "NullRptEntry";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullRptEntry::getDescription() {
    return "The <i>RptEntry</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullRptEntry::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullRptEntry::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntryIdT nextEntryId;

    SaErrorT error = saHpiRptEntryGet(sessionId, SAHPI_FIRST_ENTRY,
                                      &nextEntryId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RPT_ENTRY_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


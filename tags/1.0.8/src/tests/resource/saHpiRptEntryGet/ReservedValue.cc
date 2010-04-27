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


#include "ReservedValue.h"

using namespace ns_saHpiRptEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReservedValue::ReservedValue(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReservedValue::getName() {
    return "ReservedValue";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReservedValue::getDescription() {
    return "The <i>EntryId</i> is an invalid reserved value such "
           "as SAHPI_LAST_ENTRY.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReservedValue::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReservedValue::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntryIdT nextEntryId;
    SaHpiRptEntryT rptEntry;

    SaErrorT error = saHpiRptEntryGet(sessionId, SAHPI_LAST_ENTRY,
                                      &nextEntryId, &rptEntry);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RPT_ENTRY_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


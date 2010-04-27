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

#include "NullDrtEntry.h"

using namespace ns_saHpiDrtEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullDrtEntry::NullDrtEntry(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullDrtEntry::getName() {
    return "NullDrtEntry";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullDrtEntry::getDescription() {
    return "The <i>DrtEntry</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullDrtEntry::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullDrtEntry::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntryIdT nextEntryId;

    SaErrorT error = saHpiDrtEntryGet(sessionId, SAHPI_FIRST_ENTRY,
                                      &nextEntryId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, DRT_ENTRY_GET, 
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}



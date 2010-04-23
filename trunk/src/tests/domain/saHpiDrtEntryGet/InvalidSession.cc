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

#include "InvalidSession.h"

using namespace ns_saHpiDrtEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSession::InvalidSession(char *line) : HpiInvalidSessionTestCase(line) {
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSession::run() {
    HpiTestStatus status;
    SaHpiEntryIdT nextEntryId;
    SaHpiDrtEntryT drtEntry;

    SaErrorT error = saHpiDrtEntryGet(getInvalidSessionId(), SAHPI_FIRST_ENTRY,
                                      &nextEntryId, &drtEntry);
    if (error == SA_ERR_HPI_INVALID_SESSION) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, DRT_ENTRY_GET, 
                             SA_ERR_HPI_INVALID_SESSION, error);
    } 

    return status;
}


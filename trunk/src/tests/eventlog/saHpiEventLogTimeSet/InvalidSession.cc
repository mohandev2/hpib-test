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

using namespace ns_saHpiEventLogTimeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSession::InvalidSession(char *line) : HpiInvalidSessionTestCase(line) {
}

/*****************************************************************************
 * Return the precondition for this test case.
 *****************************************************************************/

const char *InvalidSession::getPrecondition() {
    return "Requires setting the Event Log Time without an\n"
           "SA_ERR_HPI_INVALID_DATA error code being returned.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSession::run() {
    HpiTestStatus status;

    SaErrorT error = saHpiEventLogTimeSet(getInvalidSessionId(),
                                          getValidResourceId(),
                                          SAHPI_TIME_MAX_RELATIVE + 1);
    if (error == SA_ERR_HPI_INVALID_DATA) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_INVALID_SESSION) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_TIME_SET, 
                             SA_ERR_HPI_INVALID_SESSION, error);
    } 

    return status;
}


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

#include "CloseTwice.h"

using namespace ns_saHpiSessionClose;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

CloseTwice::CloseTwice(char *line) : HpiGenTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *CloseTwice::getName() {
    return "CloseTwice";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *CloseTwice::getDescription() {
    return "Close the same session twice.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT CloseTwice::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_SESSION;
}

/*****************************************************************************
 * Run the test.
 *
 * Open a session and then close it twice.
 *****************************************************************************/

HpiTestStatus CloseTwice::run() {
    HpiTestStatus status;
    SaHpiSessionIdT sessionId;

    SaErrorT error = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID,
                                      &sessionId, NULL);
    if (error != SA_OK) {
        status.assertError(TRACE, SESSION_OPEN, SA_OK, error);
    } else {
        error = saHpiSessionClose(sessionId);
        if (error != SA_OK) {
            status.assertError(TRACE, SESSION_CLOSE, SA_OK, error);
        } else {
            error = saHpiSessionClose(sessionId);
            if (error == SA_ERR_HPI_INVALID_SESSION) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, SESSION_CLOSE, 
                                     SA_ERR_HPI_INVALID_SESSION, error);
            }
        }
    }

    return status;
}


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

#include "Duplicate.h"

using namespace ns_saHpiSubscribe;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Duplicate::Duplicate(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Duplicate::getName() {
    return "Duplicate";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Duplicate::getDescription() {
    return "Subscribe when a subscription is already in place for the session.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Duplicate::getExpectedReturn() {
    return SA_ERR_HPI_DUPLICATE;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Duplicate::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    SaErrorT error = saHpiSubscribe(sessionId);
    if (error != SA_OK) {
        status.assertError(TRACE, SUBSCRIBE, SA_OK, error);
    } else {
        error = saHpiSubscribe(sessionId);
        if (error == SA_ERR_HPI_DUPLICATE) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, SUBSCRIBE, SA_ERR_HPI_DUPLICATE, error);
        }
        
        error = saHpiUnsubscribe(sessionId);
        status.checkError(TRACE, UNSUBSCRIBE, error);
    }

    return status;
}


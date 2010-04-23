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

#include "Discover.h"

using namespace ns_saHpiDiscover;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Discover::Discover(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Discover::getName() {
    return "Discover";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Discover::getDescription() {
    return "Discover information about resources and associated domains.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Discover::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Discover::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    SaErrorT error = saHpiDiscover(sessionId);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, SESSION_DISCOVER, SA_OK, error);
    }

    return status;
}



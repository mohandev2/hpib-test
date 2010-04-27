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

#include "GetTimeout.h"

using namespace ns_saHpiAutoInsertTimeoutGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetTimeout::GetTimeout(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetTimeout::getName() {
    return "GetTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetTimeout::getDescription() {
    return "Retrieve the auto-insert timeout value.  This value indicates\n"
           "how long the resource will wait before the default auto-insertion\n"
           "policy is invoked.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetTimeout::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetTimeout::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiTimeoutT timeout;

    SaErrorT error = saHpiAutoInsertTimeoutGet(sessionId, &timeout);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, AUTO_INSERT_TIMEOUT_GET, SA_OK, error);
    }

    return status;
}

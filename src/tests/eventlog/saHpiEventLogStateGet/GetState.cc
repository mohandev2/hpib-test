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

#include "GetState.h"

using namespace ns_saHpiEventLogStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetState::GetState(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetState::getName() {
    return "GetState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetState::getDescription() {
    return "Retrieve the Event Log's enable state.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetState::runLogTest(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiBoolT state;

    SaErrorT error = saHpiEventLogStateGet(sessionId, resourceId, &state);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
    } 

    return status;
}


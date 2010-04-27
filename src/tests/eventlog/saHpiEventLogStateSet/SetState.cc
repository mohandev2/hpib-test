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
 *
 * Changes
 * 2009/06/23 - Lars.Wetzel@emerson.com
 *              Fix original state is restored
 */

#include "SetState.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetState::SetState(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetState::getName() {
    return "SetState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetState::getDescription() {
    return "Set the state to SAHPI_TRUE and then to SAHPI_FALSE\n"
           "and verify that the changes were successful.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetState::runLogTest(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiBoolT state;
    SaHpiBoolT savestate;
    SaHpiBoolT newState[] = { SAHPI_TRUE, SAHPI_FALSE };
    SaErrorT error;

    // save original value
    error = saHpiEventLogStateGet(sessionId, resourceId, &savestate);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
        return status;
    }
    // test
    for (int i = 0; i < 2; i++) {
        error = saHpiEventLogStateSet(sessionId, resourceId, newState[i]);
        if (error != SA_OK) {
            status.assertFailure(TRACE, EVENT_LOG_STATE_SET, SA_OK, error,
                              "New state is %s.", HpiString::boolean(newState[i]));
        } else {
            error = saHpiEventLogStateGet(sessionId, resourceId, &state);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_STATE_GET, SA_OK, error);
	    } else if (state == newState[i]) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                            "State change did not occur when setting it to %s.", 
                            HpiString::boolean(newState[i]));
            }
        }
    }
    // resstore original value
    error = saHpiEventLogStateSet(sessionId, resourceId, savestate);
    if (error != SA_OK) {
        status.assertFailure(TRACE, EVENT_LOG_STATE_SET, SA_OK, error,
                              "Restore state to %s.", HpiString::boolean(savestate));
    }
    return status;
}

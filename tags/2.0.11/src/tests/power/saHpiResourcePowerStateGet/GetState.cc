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

using namespace ns_saHpiResourcePowerStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetState::GetState(char *line) : PowerTestCase(line) {
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
    return "Retrieve the power state of an entity and verify\n"
           "that it is a valid value.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetState::getPrecondition() {
    return "Requires a resource that supports the POWER capability.";
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

HpiTestStatus GetState::runPowerTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiPowerStateT state;

    SaErrorT error = saHpiResourcePowerStateGet(sessionId,
                                                rptEntry->ResourceId,
                                                &state);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_POWER_STATE_GET, SA_OK, error);
    } else if (!isValidState(state)) {
        status.assertFailure(TRACE, "Invalid Power State: %s.",
                             HpiString::powerState(state));
    } else {
        status.assertPass();
    }

    return status;
}

/*****************************************************************************
 * Is the Power state valid or not?
 *****************************************************************************/

bool GetState::isValidState(SaHpiPowerStateT state) {
    return (state == SAHPI_POWER_OFF) ||
           (state == SAHPI_POWER_ON);
}

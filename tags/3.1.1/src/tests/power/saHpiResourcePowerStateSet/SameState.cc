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

#include "SameState.h"

using namespace ns_saHpiResourcePowerStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SameState::SameState(char *line) : PowerTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SameState::getName() {
    return "SameState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SameState::getDescription() {
    return "If the power is already in the state being set, the function\n"
           "will have no effect and will return normally (assuming there\n"
           "are no other errors).";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SameState::getPrecondition() {
    return "Requires a resource that supports the POWER capability and\n"
           "is not the resource the HPI User is running on.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SameState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SameState::runPowerTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiPowerStateT state;
    SaHpiResourceIdT hpiResourceId;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    SaErrorT error = getHpiUserResource(sessionId, &hpiResourceId);
    if (error != SA_OK) {
        status.assertError(TRACE, RESOURCE_ID_GET, SA_OK, error);
    } else if (resourceId == hpiResourceId) {
        status.assertNotSupport();
    } else {
        error = getPowerState(sessionId, resourceId, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_GET, SA_OK, error);
        } else if (state == SAHPI_POWER_ON) {
            status.add(TRACE, testOn(sessionId, resourceId));
            if (status.isOkay()) {
                status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_OFF));
                if (status.isOkay()) {
                    status.add(TRACE, testOff(sessionId, resourceId));

                    // restore power to ON
                    status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_ON));
                }
            }
        } else { // OFF
            status.add(TRACE, testOff(sessionId, resourceId));
            if (status.isOkay()) {
                status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_ON));
                if (status.isOkay()) {
                    status.add(TRACE, testOn(sessionId, resourceId));

                    // restore power to OFF
                    status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_OFF));
                }
            }
        }

        if (status.isOkay()) {
            status.assertPass();
        }
    }

    return status;
}

/*****************************************************************************
 * Test Power On.  The entity is already on and we are setting to on.
 *****************************************************************************/

HpiTestStatus SameState::testOn(SaHpiSessionIdT sessionId,
                                SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiPowerStateT state;

    SaErrorT error = setPowerState(sessionId, resourceId, SAHPI_POWER_ON);
    if (error == SA_ERR_HPI_INVALID_REQUEST) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error,
                            "Unable to set power state to the same value of %s.", 
                            HpiString::powerState(SAHPI_POWER_ON));
    } else {
        error = getPowerState(sessionId, resourceId, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_GET, SA_OK, error);
        } else if (state != SAHPI_POWER_ON) {
            status.assertFailure(TRACE, "Setting power state to same value of ON failed.");
        }
    }

    return status;
}

/*****************************************************************************
 * Test Power Off.  The entity is already off and we are setting to off.
 *****************************************************************************/

HpiTestStatus SameState::testOff(SaHpiSessionIdT sessionId,
                                 SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiPowerStateT state;

    SaErrorT error = setPowerState(sessionId, resourceId, SAHPI_POWER_OFF);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error,
                            "Unable to set power state to the same value of %s.", 
                            HpiString::powerState(SAHPI_POWER_OFF));
    } else {
        error = getPowerState(sessionId, resourceId, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_GET, SA_OK, error);
        } else if (state != SAHPI_POWER_OFF) {
            status.assertFailure(TRACE, "Setting power state to same value of OFF failed.");
        }
    }

    return status;
}

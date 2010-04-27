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

#include "PowerCycleOn.h"

using namespace ns_saHpiResourcePowerStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PowerCycleOn::PowerCycleOn(char *line) : PowerTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *PowerCycleOn::getName() {
    return "PowerCycleOn";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *PowerCycleOn::getDescription() {
    return "Power Cycle an entity that is ON.  If the entity is already\n"
           "ON, it will be power cycled.  For entities that are OFF, they\n"
           "will be turned ON and then power cycled.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *PowerCycleOn::getPrecondition() {
    return "Requires a resource that supports the POWER capability and\n"
           "is not the resource the HPI User is running on.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT PowerCycleOn::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus PowerCycleOn::runPowerTest(SaHpiSessionIdT sessionId,
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
            status.add(TRACE, testPowerCycle(sessionId, resourceId));
        } else { // OFF
            status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_ON));
            if (status.isOkay()) {
                status.add(TRACE, testPowerCycle(sessionId, resourceId));

                // restore power to off
                status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_OFF));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test the Power Cycle of an entity.  Verify that the entity is turned
 * off and then back on.
 *****************************************************************************/

HpiTestStatus PowerCycleOn::testPowerCycle(SaHpiSessionIdT sessionId,
                                           SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    bool successOn;
    bool successOff;

    SaErrorT error = setPowerState(sessionId, resourceId, SAHPI_POWER_CYCLE);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, 
                             SA_OK, error, "Unable to power cycle.");
    } else {
        status.add(TRACE, pollForPowerOff(sessionId, resourceId, &successOff));
        if (status.isOkay()) {
            pauseForPowerCycleTransition();
            status.add(TRACE, pollForPowerOn(sessionId, resourceId, &successOn));
            if (status.isOkay()) {
                if (!successOn) {
                    status.assertFailure(TRACE, "Power Cycle failed to turn entity ON.");
                } else if (!successOff) {
                    status.assertPassWarn(TRACE, "Failed to confirm that power was turned off. "
                                                 "Power was on at completion of the Power Cycle.");
                } else {
                    status.assertPass();
                }
            }
        }
    }

    return status;
}


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

#include "PowerCycleOff.h"

using namespace ns_saHpiResourcePowerStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PowerCycleOff::PowerCycleOff(char *line) : PowerTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *PowerCycleOff::getName() {
    return "PowerCycleOff";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *PowerCycleOff::getDescription() {
    return "Power Cycle an entity that is OFF.  If the entity is already\n"
           "OFF, it will be power cycled.  For entities that are ON, they\n"
           "will be turned OFF and then power cycled.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *PowerCycleOff::getPrecondition() {
    return "Requires a resource that supports the POWER capability and\n"
           "is not the resource the HPI User is running on.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT PowerCycleOff::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus PowerCycleOff::runPowerTest(SaHpiSessionIdT sessionId,
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
        } else if (state == SAHPI_POWER_OFF) {
            status.add(TRACE, testPowerCycle(sessionId, resourceId));

            // restore to off
            status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_OFF));

        } else { // ON
            status.add(TRACE, setPowerStateAndWait(sessionId, resourceId, SAHPI_POWER_OFF));
            if (status.isOkay()) {
                status.add(TRACE, testPowerCycle(sessionId, resourceId));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test the Power Cycle.  Verify that the resource is turned on.
 *****************************************************************************/

HpiTestStatus PowerCycleOff::testPowerCycle(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    bool success;

    SaErrorT error = setPowerState(sessionId, resourceId, SAHPI_POWER_CYCLE);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, 
                             SA_OK, error, "Unable to power cycle.");
    } else {
        status.add(TRACE, pollForPowerOn(sessionId, resourceId, &success));
        if (status.isOkay()) {
            if (success) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, "Power Cycle didn't turn entity ON.");
            }
        }
    }

    return status;
}


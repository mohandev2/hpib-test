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

#include "PowerToggle.h"

using namespace ns_saHpiResourcePowerStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PowerToggle::PowerToggle(char *line) : PowerTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *PowerToggle::getName() {
    return "PowerToggle";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *PowerToggle::getDescription() {
    return "Toggle the power for a resource.  If it is ON, turn it OFF\n"
           "and then ON.  If it is OFF, turn it ON and then OFF.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *PowerToggle::getPrecondition() {
    return "Requires a resource that supports the POWER capability and\n"
           "is not the resource the HPI User is running on.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT PowerToggle::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus PowerToggle::runPowerTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiPowerStateT state;
    SaHpiResourceIdT hpiResourceId;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    bool success;

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
            error = setPowerState(sessionId, resourceId, SAHPI_POWER_OFF);
            if (error != SA_OK) {
                status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error);
            } else {
                status.add(TRACE, pollForPowerOff(sessionId, resourceId, &success));
                if (status.isOkay()) {
                    if (!success) {
                        status.assertFailure(TRACE, "Setting power to OFF failed.");
                    } else {
                        error = setPowerState(sessionId, resourceId, SAHPI_POWER_ON);
                        if (error == SA_ERR_HPI_INVALID_REQUEST) {
                            status.assertNotSupport();
                        } else if (error != SA_OK) {
                            status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error);
                        } else {
                            status.add(TRACE, pollForPowerOn(sessionId, resourceId, &success));
                            if (status.isOkay()) {
                                if (!success) {
                                    status.assertFailure(TRACE, "Setting power to ON failed.");
                                } else {
                                    status.assertPass();
                                }
                            }
                        }
                    }
                }
            }
        } else { // OFF
            error = setPowerState(sessionId, resourceId, SAHPI_POWER_ON);
            if (error == SA_ERR_HPI_INVALID_REQUEST) {
                status.assertNotSupport();
            } else if (error != SA_OK) {
                status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error);
            } else {
                status.add(TRACE, pollForPowerOn(sessionId, resourceId, &success));
                if (status.isOkay()) {
                    if (!success) {
                        status.assertFailure(TRACE, "Setting power to ON failed.");
                    } else {
                        error = setPowerState(sessionId, resourceId, SAHPI_POWER_OFF);
                        if (error != SA_OK) {
                            status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error);
                        } else {
                            status.add(TRACE, pollForPowerOff(sessionId, resourceId, &success));
                            if (status.isOkay()) {
                                if (!success) {
                                    status.assertFailure(TRACE, "Setting power to OFF failed.");
                                } else {
                                    status.assertPass();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return status;
}

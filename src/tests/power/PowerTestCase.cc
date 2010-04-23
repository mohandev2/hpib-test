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

#include "PowerTestCase.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_BUSY_RETRY 8
#define POLL_TIME 100 // msec

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PowerTestCase::PowerTestCase() : HpiResourceTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PowerTestCase::PowerTestCase(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Only run the test on resources that support Power.
 *****************************************************************************/

HpiTestStatus PowerTestCase::runResourceTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasPowerCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        status = runPowerTest(sessionId, rptEntry);
    }

    return status;
}

/*****************************************************************************
 * Get the power state.  If the entity is busy, try a number of times.
 *****************************************************************************/

SaErrorT PowerTestCase::getPowerState(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiPowerStateT *state) {
    int busyCount = 0;

    SaErrorT error = saHpiResourcePowerStateGet(sessionId, resourceId, state);
    while (error == SA_ERR_HPI_BUSY && busyCount < MAX_BUSY_RETRY) {
        busyCount++;
        secSleep(1);
        error = saHpiResourcePowerStateGet(sessionId, resourceId, state);
    }

    return error;
}

/*****************************************************************************
 * Set the power state.  If the entity is busy, try a number of times.
 *****************************************************************************/

SaErrorT PowerTestCase::setPowerState(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiPowerStateT state) {
    int busyCount = 0;

    SaErrorT error = saHpiResourcePowerStateSet(sessionId, resourceId, state);
    while (error == SA_ERR_HPI_BUSY && busyCount < MAX_BUSY_RETRY) {
        busyCount++;
        secSleep(1);
        error = saHpiResourcePowerStateSet(sessionId, resourceId, state);
    }

    return error;
}

/*****************************************************************************
 * Set the power to on or off and then wait for the change to occur.
 *****************************************************************************/

HpiTestStatus PowerTestCase::setPowerStateAndWait(SaHpiSessionIdT sessionId,
                                                  SaHpiResourceIdT resourceId,
                                                  SaHpiPowerStateT state) {
    HpiTestStatus status;
    bool success;

    if (state == SAHPI_POWER_ON) {
        SaErrorT error = setPowerState(sessionId, resourceId, state);
        if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error);
        } else {
            status.add(TRACE, pollForPowerOn(sessionId, resourceId, &success));
            if (status.isOkay()) {
                if (!success) {
                    status.assertError(TRACE, "Unable to turn power on.");
                }
            }
        }
    } else { // OFF
        SaErrorT error = setPowerState(sessionId, resourceId, state);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_SET, SA_OK, error);
        } else {
            status.add(TRACE, pollForPowerOff(sessionId, resourceId, &success));
            if (status.isOkay()) {
                if (!success) {
                    status.assertError(TRACE, "Unable to turn power off.");
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Poll for the power to turn off.  The PowerOffTime is the maximum
 * time it takes to power down an entity.
 *****************************************************************************/

HpiTestStatus PowerTestCase::pollForPowerOff(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId,
                                             bool *success) {
    HpiTestStatus status;
    SaHpiPowerStateT state;

    *success = false;
    TimeMsec duration = getPowerOffTime() + POLL_TIME;
    while (duration > 0 && !(*success) && status.isOkay()) {
        TimeMsec startTime = Time::getCurrent();

        SaErrorT error = getPowerState(sessionId, resourceId, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_GET, SA_OK, error);
        } else if (state == SAHPI_POWER_OFF) {
            *success = true;
        } else {
            TimeMsec elapsedTime = Time::getCurrent() - startTime;
            if (elapsedTime >= POLL_TIME) {
                duration -= elapsedTime;
            } else {
                duration -= POLL_TIME;
                if (duration > 0) {
                    msecSleep(POLL_TIME - elapsedTime);
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Poll for the power to turn on.  The PowerOnTime is the maximum
 * time it takes to power up an entity.
 *****************************************************************************/

HpiTestStatus PowerTestCase::pollForPowerOn(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId,
                                            bool *success) {
    HpiTestStatus status;
    SaHpiPowerStateT state;

    *success = false;
    TimeMsec duration = getPowerOnTime() + POLL_TIME;
    while (duration > 0 && !(*success) && status.isOkay()) {
        TimeMsec startTime = Time::getCurrent();

        SaErrorT error = getPowerState(sessionId, resourceId, &state);
        if (error != SA_OK) {
            status.assertError(TRACE, RESOURCE_POWER_STATE_GET, SA_OK, error);
        } else if (state == SAHPI_POWER_ON) {
            *success = true;
        } else {
            TimeMsec elapsedTime = Time::getCurrent() - startTime;
            if (elapsedTime >= POLL_TIME) {
                duration -= elapsedTime;
            } else {
                duration -= POLL_TIME;
                if (duration > 0) {
                    msecSleep(POLL_TIME - elapsedTime);
                }
            }
        }
    }

    return status;
}


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

#include "RemoveAllStates.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

RemoveAllStates::RemoveAllStates(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *RemoveAllStates::getName() {
    return "RemoveAllStates";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *RemoveAllStates::getDescription() {
    return "Use SAHPI_ALL_EVENT_STATES to remove all of the event states\n"
           "supported by the sensor.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *RemoveAllStates::getPrecondition() {
    return "Requires a sensor whose EventCtrl is SAHPI_SEC_PER_EVENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT RemoveAllStates::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus RemoveAllStates::runSensorTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, saveEventMasks(sessionId,
                                         rptEntry->ResourceId,
                                         sensorRec->Num)); 
        if (status.isOkay()) {
            SaErrorT error = saHpiSensorEventMasksSet(sessionId,
                                             rptEntry->ResourceId,
                                             sensorRec->Num, 
                                             SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS, 
                                             SAHPI_ALL_EVENT_STATES,
                                             SAHPI_ALL_EVENT_STATES); 
            if (error != SA_OK) {
                status.assertFailure(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
            } else {
                error = saHpiSensorEventMasksGet(sessionId,
                                                 rptEntry->ResourceId,
                                                 sensorRec->Num, 
                                                 &assertEventMask,
                                                 &deassertEventMask); 
                if (error != SA_OK) {
                    status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
                } else if (assertEventMask != 0) {
                    status.assertFailure(TRACE, 
                            "Failed to clear AssertEventMask [0x%X].",
                            assertEventMask);
                } else if (deassertEventMask != 0) {
                    status.assertFailure(TRACE, 
                            "Failed to clear DeassertEventMask [0x%X].",
                            deassertEventMask);
                } else {
                    status.assertPass();
                }
            }

            status.add(TRACE, restoreEventMasks(sessionId,
                                                rptEntry->ResourceId,
                                                sensorRec->Num)); 
        }
    }

    return status;
}


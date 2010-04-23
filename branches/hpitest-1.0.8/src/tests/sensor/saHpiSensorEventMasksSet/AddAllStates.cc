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

#include "AddAllStates.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddAllStates::AddAllStates(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddAllStates::getName() {
    return "AddAllStates";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddAllStates::getDescription() {
    return "Use SAHPI_ALL_EVENT_STATES to add all of the event states "
           "supported by the sensor.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AddAllStates::getPrecondition() {
    return "Requires a sensor whose event masks can be set, i.e. "
           "EventCtrl is not SAHPI_SEC_PER_EVENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddAllStates::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddAllStates::runSensorTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, saveEventMasks(sessionId,
                                         rptEntry->ResourceId, sensorRec->Num));
        if (status.isOkay()) {

            // Clear the event masks

            SaErrorT error = saHpiSensorEventMasksSet(sessionId, rptEntry->ResourceId,
                                                      sensorRec->Num, 
                                                      SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS, 
                                                      sensorRec->Events, sensorRec->Events); 
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
            } else {

                // Set all of the supported event states.

                error = saHpiSensorEventMasksSet(sessionId, rptEntry->ResourceId,
                                                 sensorRec->Num, 
                                                 SAHPI_SENS_ADD_EVENTS_TO_MASKS, 
                                                 SAHPI_ALL_EVENT_STATES,
                                                 SAHPI_ALL_EVENT_STATES); 
                if (error != SA_OK) {
                    status.assertFailure(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
                } else {

                    // Verify that the event masks were set correctly.

                    error = saHpiSensorEventMasksGet(sessionId,
                                                     rptEntry->ResourceId,
                                                     sensorRec->Num, 
                                                     &assertEventMask,
                                                     &deassertEventMask); 
                    if (error != SA_OK) {
                        status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
                    } else if (assertEventMask != sensorRec->Events) {
                        status.assertFailure(TRACE, 
                               "AssertEventMask [0x%X] is not equal to Sensor RDR Events [0x%X].",
                               assertEventMask, sensorRec->Events);
                    } else if (deassertEventMask != sensorRec->Events) {
                        status.assertFailure(TRACE, 
                               "DeassertEventMask [0x%X] is not equal to Sensor RDR Events [0x%X].",
                               deassertEventMask, sensorRec->Events);
                    } else {
                        status.assertPass();
                    }
                }
            }

            status.add(TRACE, restoreEventMasks(sessionId,
                                                rptEntry->ResourceId, sensorRec->Num));
        }
    }

    return status;
}

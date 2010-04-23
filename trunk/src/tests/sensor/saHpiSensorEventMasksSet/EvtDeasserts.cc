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

#include "EvtDeasserts.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EvtDeasserts::EvtDeasserts(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EvtDeasserts::getName() {
    return "EvtDeasserts";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *EvtDeasserts::getDescription() {
    return "When the <i>AssertEventMask</i> and <i>DeassertEventMask</i> are\n"
           "not independent, the <i>DeassertEventMask</i> is set according the\n"
           "<i>AssertEventMask</i>.  Test clearing the <i>AssertEventMask</i>\n"
           "and then setting it to verify that the <i>DeassertEventMask</i> also\n" 
           "is assigned the same values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *EvtDeasserts::getPrecondition() {
    return "Requires a sensor on a resource that has the\n"
           "SAHPI_CAPABILITY_EVT_DEASSERTS flag set in its RPT entry.\n"
           "Must also be able to change that sensor's event masks.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EvtDeasserts::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus EvtDeasserts::runSensorTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    if (!hasEvtDeassertsCapability(rptEntry)) {
        status.assertNotSupport();
    } else if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, saveEventMasks(sessionId,
                                         rptEntry->ResourceId,
                                         sensorRec->Num));
        if (status.isOkay()) {

            // Clear the Assert Event Mask which should also 
            // clear the Deasserts Event Mask.

            SaErrorT error = saHpiSensorEventMasksSet(sessionId,
                                                      rptEntry->ResourceId,
                                                      sensorRec->Num, 
                                                      SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS, 
                                                      SAHPI_ALL_EVENT_STATES, 0x0); 
            if (error != SA_OK) {
                status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
            } else {

                // Verify that the Deasserts Event Mask was cleared.

                error = saHpiSensorEventMasksGet(sessionId,
                                                 rptEntry->ResourceId,
                                                 sensorRec->Num, 
                                                 &assertEventMask,
                                                 &deassertEventMask); 
                if (error == SA_ERR_HPI_BUSY) {
                    status.assertNotSupport();
                } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                    status.assertNotSupport();
                } else if (error != SA_OK) {
                    status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
                } else if (deassertEventMask != 0) {
                    status.assertFailure(TRACE, "DeassertEventMask was not cleared.");
                } else {

                    // Now add event states to the Assert Event Mask, which should also
                    // change the Deassert Event Mask.

                    error = saHpiSensorEventMasksSet(sessionId,
                                                     rptEntry->ResourceId,
                                                     sensorRec->Num, 
                                                     SAHPI_SENS_ADD_EVENTS_TO_MASKS, 
                                                     sensorRec->Events, 0x0); 
                    if (error != SA_OK) {
                        status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
                    } else {

                        // Verify that the Deasserts Event Mask was correctly set.

                        error = saHpiSensorEventMasksGet(sessionId,
                                                         rptEntry->ResourceId,
                                                         sensorRec->Num, 
                                                         &assertEventMask,
                                                         &deassertEventMask); 
                        if (error == SA_ERR_HPI_BUSY) {
                            status.assertNotSupport();
                        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                            status.assertNotSupport();
                        } else if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
                        } else if (deassertEventMask == sensorRec->Events) {
                            status.assertPass();
                        } else {
                            status.assertFailure(TRACE, 
                                    "DeassertEventMask [0x%X] is not equal to the Sensor RDR's Events [0x%X]", 
                                    deassertEventMask, sensorRec->Events);
                        } 
                    }
                }
            }

            // restore original event masks

            status.add(TRACE, restoreEventMasks(sessionId, rptEntry->ResourceId, sensorRec->Num));
        }
    }

    return status;
}

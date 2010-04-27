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

#include "SetMasks.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetMasks::SetMasks(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetMasks::getName() {
    return "SetMasks";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetMasks::getDescription() {
    return "Clear the Assert and Deassert Event Masks and\n"
           "then restore the values to their original values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetMasks::getPrecondition() {
    return "Requires a sensor whose EventCtrl is SAHPI_SEC_PER_EVENT and "
           "that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetMasks::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetMasks::runSensorTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT origAssertEventMask;
    SaHpiEventStateT origDeassertEventMask;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEventMasksGet(sessionId,
                                                  rptEntry->ResourceId,
                                                  sensorRec->Num, 
                                                  &origAssertEventMask,
                                                  &origDeassertEventMask); 
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
        } else {
            error = saHpiSensorEventMasksSet(sessionId,
                                             rptEntry->ResourceId,
                                             sensorRec->Num, 
                                             SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS, 
                                             origAssertEventMask,
                                             origDeassertEventMask); 
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
                        "Failed to clear AssertEventMask [0x%X].", assertEventMask);
                } else if (deassertEventMask != 0) {
                    status.assertFailure(TRACE,
                        "Failed to clear DeassertEventMask [0x%X].", deassertEventMask);
                } else {
                    error = saHpiSensorEventMasksSet(sessionId,
                                                     rptEntry->ResourceId,
                                                     sensorRec->Num, 
                                                     SAHPI_SENS_ADD_EVENTS_TO_MASKS, 
                                                     origAssertEventMask,
                                                     origDeassertEventMask); 
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
                        } else if (assertEventMask != origAssertEventMask) {
                            status.assertFailure(TRACE, 
                                "Failed to restore AssertEventMask [0x%X] to [0x%X].",
                                assertEventMask, origAssertEventMask);
                        } else if (deassertEventMask != origDeassertEventMask) {
                            status.assertFailure(TRACE, 
                                "Failed to restore DeassertEventMask [0x%X] to [0x%X].",
                                deassertEventMask, origDeassertEventMask);
                        } else {
                            status.assertPass();
                        }
                    }
                }
            }
        }
    }

    return status;
}

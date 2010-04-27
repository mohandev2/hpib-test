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

#include "NotEvtDeasserts.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NotEvtDeasserts::NotEvtDeasserts(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NotEvtDeasserts::getName() {
    return "NotEvtDeasserts";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NotEvtDeasserts::getDescription() {
    return "For independent Assert and Deassert Event Masks, verify\n"
           "that event states can be independently added and removed.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NotEvtDeasserts::getPrecondition() {
    return "Requires a sensor who meets the following conditions:\n"
           "<ol>\n"
           "<li>It's resource does not support SAHPI_CAPABILITY_EVT_DEASSERTS.</li>\n"
           "<li>It's EventCtrl is SAHPI_SEC_PER_EVENT.</li>\n"
           "<li>It's Events is not 0x0.</li>\n"
           "<li>It does not respond with SA_ERR_HPI_ENTITY_NOT_PRESENT.</li>\n"
           "</ol>\n";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NotEvtDeasserts::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the tests.
 *
 * First, set the Assert Event Mask to the full set of supported event states
 * and clear the Deassert Event Mask.  Then do the reverse.  In each case,
 * verify that the changes occurred correctly.
 *****************************************************************************/

HpiTestStatus NotEvtDeasserts::runSensorTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    if (hasEvtDeassertsCapability(rptEntry)) {
        status.assertNotSupport();
    } else if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else if (sensorRec->Events == 0x0) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, saveEventMasks(sessionId,
                                         rptEntry->ResourceId,
                                         sensorRec->Num));
        if (status.isOkay()) {
            status.add(TRACE, setMasks(sessionId, rptEntry->ResourceId,
                                       sensorRec->Num, sensorRec->Events, 0x0));
            if (status.isOkay()) {
                SaErrorT error = saHpiSensorEventMasksGet(sessionId,
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
                } else if (assertEventMask != sensorRec->Events) {
                    status.assertFailure(TRACE, 
                            "AssertEventMask [0x%X] is not equal to the Sensor RDR's Events [0x%X].",
                            assertEventMask, sensorRec->Events);
                } else if (deassertEventMask != 0) {
                    status.assertFailure(TRACE, 
                            "DeassertEventMask [0x%X] was not cleared.", deassertEventMask);
                } else {
                    status.add(TRACE, setMasks(sessionId, rptEntry->ResourceId,
                                               sensorRec->Num, 0x0, sensorRec->Events));
                    if (status.isOkay()) {
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
                        } else if (assertEventMask != 0x0) {
                            status.assertFailure(TRACE, 
                                    "AssertEventMask [0x%X] was not cleared.", assertEventMask);
                        } else if (deassertEventMask != sensorRec->Events) {
                            status.assertFailure(TRACE, 
                                    "DeassertEventMask [0x%X] is not equal to the Sensor RDR's Events [0x%X].",
                                    deassertEventMask, sensorRec->Events);
                        } else {
                            status.assertPass();
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

/*****************************************************************************
 * Set the event masks to the given event masks.
 *****************************************************************************/

HpiTestStatus NotEvtDeasserts::setMasks(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiSensorNumT sensorNum, 
                                        SaHpiEventStateT assertEventMask,
                                        SaHpiEventStateT deassertEventMask) {
    HpiTestStatus status;

    // First, clear of the states from the masks.

    SaErrorT error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum,
                                              SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS,
                                              SAHPI_ALL_EVENT_STATES, SAHPI_ALL_EVENT_STATES);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
    } else {

        // Secondly, set the event states from the given masks.

        error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum, 
                                         SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                         assertEventMask, deassertEventMask);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
        }
    }

    return status;
}

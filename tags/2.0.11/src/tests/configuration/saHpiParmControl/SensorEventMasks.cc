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

#include "SensorEventMasks.h"

using namespace ns_saHpiParmControl;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_SENSORS 2000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorEventMasks::SensorEventMasks(char *line) : ParmControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SensorEventMasks::getName() {
    return "SensorEventMasks";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SensorEventMasks::getDescription() {
    return "For a resource that supports sensors, do the following:\n"
           "<ol>\n"
           "<li>Save the resource's configuraton parameters.</li>\n"
           "<li>For each sensor's event mask that can be changed, \n"
                "remove all of the event states.</li>\n"
           "<li>Restore the resource's configuration parameters.</li>\n"
           "<li>Verify that the event masks were restored for each sensor.</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SensorEventMasks::getPrecondition() {
    return "Requires a resource that supports Configuration and has one\n"
           "or more sensors whose event masks can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SensorEventMasks::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SensorEventMasks::runParmTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiSensorNumT sensorNum[MAX_SENSORS];
    SaHpiEventStateT origAssertEventMask[MAX_SENSORS];
    SaHpiEventStateT origDeassertEventMask[MAX_SENSORS];
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;
    int sensorCount = 0;

    if (!hasSensorCapability(rptEntry)) {
        status.assertNotSupport();
    } else {

        // Save the enabled state for each sensor on the resource.

        SaErrorT error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_SAVE_PARM);
        if (error != SA_OK) {
            status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Saving parameters.");
        } else {

            // Save the event masks for each sensor and change it

            SaHpiRdrT rdr;
            SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
            while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY && sensorCount < MAX_SENSORS) {
                SaHpiEntryIdT entryId = nextEntryId;
                error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                    entryId, &nextEntryId, &rdr);
                if (error == SA_ERR_HPI_NOT_PRESENT) {
                    break;
                } else if (error != SA_OK) {
                    status.assertError(TRACE, RDR_GET, SA_OK, error);
                    break;
                } else if (rdr.RdrType == SAHPI_SENSOR_RDR) {
                    SaHpiSensorRecT *sensorRec = &(rdr.RdrTypeUnion.SensorRec);
                    if (sensorRec->EventCtrl == SAHPI_SEC_PER_EVENT) {
                        sensorNum[sensorCount] = sensorRec->Num;
                        error = saHpiSensorEventMasksGet(sessionId, resourceId, sensorRec->Num, 
                                                         &origAssertEventMask[sensorCount],
                                                         &origDeassertEventMask[sensorCount]);
                        if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
                        } else {
                            error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorRec->Num, 
                                                             SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS, 
                                                             SAHPI_ALL_EVENT_STATES,
                                                             SAHPI_ALL_EVENT_STATES);
                            if (error != SA_OK) {
                                status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
                            } else {
                                sensorCount++;
                            }
                        }
                    }
                }
            }

            if (sensorCount == 0) {
                status.assertNotSupport();
            }

            // Restore the enabled state for each sensor and verify that the restoration worked.

            if (status.isOkay()) {
                error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_RESTORE_PARM);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Restoring parameters.");
                } else {
                    for (int i = 0; i < sensorCount && status.isOkay(); i++) {
                        pushLocation("Sensor", sensorNum[i]);
                        error = saHpiSensorEventMasksGet(sessionId, resourceId, sensorNum[i], 
                                                         &assertEventMask, &deassertEventMask);
                        if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
                        } else if (assertEventMask != origAssertEventMask[i]) {
                            status.assertFailure(TRACE, "Sensor AssertEventMask was not restored!");
                        } else if (deassertEventMask != origDeassertEventMask[i]) {
                            status.assertFailure(TRACE, "Sensor DeassertEventMask was not restored!");
                        } else {
                            status.assertPass();
                        }
                        popLocation();
                     }
                }
            }

            // force a restoration for each sensor's enabled state in case something went wrong above.

            for (int i = 0; i < sensorCount; i++) {
                error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum[i], 
                                                 SAHPI_SENS_ADD_EVENTS_TO_MASKS, 
                                                 origAssertEventMask[i], origDeassertEventMask[i]);
                status.checkError(TRACE, SENSOR_EVENT_MASKS_SET, error);
            }
        }
    }

    return status;
}

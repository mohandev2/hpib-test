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

#include "SensorEnableState.h"

using namespace ns_saHpiParmControl;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_SENSORS 2000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorEnableState::SensorEnableState(char *line) : ParmControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SensorEnableState::getName() {
    return "SensorEnableState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SensorEnableState::getDescription() {
    return "For a resource that supports sensors, do the following:\n"
           "<ol>\n"
           "<li>Save the resource's configuraton parameters.</li>\n"
           "<li>For each sensor that can be changed, toggle it's "
               "enabled state.</li>\n"
           "<li>Restore the resource's configuration parameters.</li>\n"
           "<li>Verify that the enabled state was restored for each sensor.</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SensorEnableState::getPrecondition() {
    return "Requires a resource that supports Configuration and has one\n"
           "or more sensors that can be enabled/disabled.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SensorEnableState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SensorEnableState::runParmTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiSensorNumT sensorNum[MAX_SENSORS];
    SaHpiBoolT origEnabled[MAX_SENSORS];
    SaHpiBoolT enabled;
    int sensorCount = 0;

    if (!hasSensorCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        // Save the enabled state for each sensor on the resource.

        SaErrorT error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_SAVE_PARM);
        if (error != SA_OK) {
            status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Saving parameters.");
        } else {

            // Save the enabled state for each sensor and toggle it's state.

            SaHpiRdrT rdr;
            SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
            while (!(status.hasFault()) && 
                    (nextEntryId != SAHPI_LAST_ENTRY) && 
                    (sensorCount < MAX_SENSORS)) {

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
                    if (sensorRec->EnableCtrl) {
                        sensorNum[sensorCount] = sensorRec->Num;
                        error = saHpiSensorEnableGet(sessionId, resourceId,
                                                     sensorRec->Num, &origEnabled[sensorCount]);
                        if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
                        } else {
                            sensorCount++;
                            error = saHpiSensorEnableSet(sessionId, resourceId,
                                                         sensorRec->Num,
                                                         !origEnabled[sensorCount]);
                            if (error != SA_OK) {
                                status.assertError(TRACE, SENSOR_ENABLE_SET, SA_OK, error);
                            }
                        }
                    }
                }
            }

            // If we didn't change any of the sensors, then
            // we can't continue with the test.

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
                        error = saHpiSensorEnableGet(sessionId, resourceId, sensorNum[i], &enabled);
                        if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
                        } else if (!isBoolEqual(origEnabled[i], enabled)) {
                            status.assertFailure(TRACE, "Sensor Enabled state was not restored!");
                        } else {
                            status.assertPass();
                        }
                        popLocation();
                     }
                }
            }

            // force a restoration for each sensor's enabled state in case something went wrong above.

            for (int i = 0; i < sensorCount; i++) {
                error = saHpiSensorEnableSet(sessionId, resourceId, sensorNum[i], origEnabled[i]);
                status.checkError(TRACE, SENSOR_ENABLE_SET, error);
            }
        }
    }

    return status;
}


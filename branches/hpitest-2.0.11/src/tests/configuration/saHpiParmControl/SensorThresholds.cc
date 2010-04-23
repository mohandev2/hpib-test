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

#include "SensorThresholds.h"
#include "SensorHelper.h"

using namespace ns_saHpiParmControl;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_SENSORS 2000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorThresholds::SensorThresholds(char *line) : ParmControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SensorThresholds::getName() {
    return "SensorThresholds";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SensorThresholds::getDescription() {
    return "For a resource that supports threshold sensors, do the following:\n"
           "<ol>\n"
           "<li>Save the resource's configuraton parameters.</li>\n"
           "<li>Change the UpMinor value to the same value of UpMajor.</li>\n"
           "<li>Restore the resource's configuration parameters.</li>\n"
           "<li>Verify that the UpMinor was restored for each sensor.</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SensorThresholds::getPrecondition() {
    return "Requires a resource that supports Configuration and has one or\n"
           "threshold sensors where the UP MINOR threshold is read/write and\n"
           "the UP MAJOR threshold is readable.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SensorThresholds::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SensorThresholds::runParmTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiSensorNumT sensorNum[MAX_SENSORS];
    SaHpiSensorReadingT origUpMinor[MAX_SENSORS];
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThresholdsT newSensorThresholds;
    int sensorCount = 0;

    if (!hasSensorCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        // Save the enabled state for each sensor on the resource.

        SaErrorT error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_SAVE_PARM);
        if (error != SA_OK) {
            status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Saving parameters.");
        } else {

            // Save the thresholds for each sensor and change UpMinor

            SaHpiRdrT rdr;
            SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
            while (!status.hasFault() && 
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
                    if (sensorRec->ThresholdDefn.IsAccessible &&
                        SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_UP_MINOR) &&
                        SensorHelper::isReadThreshold(sensorRec, SAHPI_STM_UP_MAJOR)) {

                        sensorNum[sensorCount] = sensorRec->Num;
                        error = saHpiSensorThresholdsGet(sessionId, resourceId, 
                                                         sensorRec->Num, &sensorThresholds);
                        if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
                        } else {
                            origUpMinor[sensorCount] = sensorThresholds.UpMinor;
                            sensorCount++;

                            SensorHelper::fill(&newSensorThresholds, sensorThresholds.UpMinor.Type);
                            SensorHelper::setThreshold(&newSensorThresholds,
                                                       SAHPI_STM_UP_MINOR,
                                                       sensorThresholds.UpMajor);
                            
                            error = saHpiSensorThresholdsSet(sessionId, resourceId,
                                                             sensorRec->Num,
                                                             &newSensorThresholds);
                            if (error != SA_OK) {
                                status.assertError(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
                            }
                        }
                    }
                }
            }

            if (sensorCount == 0) {
                status.assertNotSupport();
            }

            // Restore the thresholds for each sensor and verify that the restoration worked.

            if (status.isOkay()) {
                error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_RESTORE_PARM);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Restoring parameters.");
                } else {
                    for (int i = 0; i < sensorCount && status.isOkay(); i++) {
                        pushLocation("Sensor", sensorNum[i]);
                        error = saHpiSensorThresholdsGet(sessionId, resourceId,
                                                         sensorNum[i], &sensorThresholds);
                        if (error != SA_OK) {
                            status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
                        } else if (SensorHelper::compare(sensorThresholds.UpMinor, origUpMinor[i]) != 0) {
                            status.assertFailure(TRACE, "Sensor UpMinor Threshold was not restored!");
                        } else {
                            status.assertPass();
                        }
                        popLocation();
                     }
                }
            }

            // force a restoration for each sensor's threshold in case something went wrong above.

            for (int i = 0; i < sensorCount; i++) {
                SensorHelper::fill(&sensorThresholds, origUpMinor[i].Type);
                SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_UP_MINOR, origUpMinor[i]);

                error = saHpiSensorThresholdsSet(sessionId, resourceId,
                                                 sensorNum[i], &sensorThresholds);
                status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);
            }
        }
    }

    return status;
}


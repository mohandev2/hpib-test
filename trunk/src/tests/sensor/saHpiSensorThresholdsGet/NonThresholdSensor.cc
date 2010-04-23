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

#include "NonThresholdSensor.h"

using namespace ns_saHpiSensorThresholdsGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NonThresholdSensor::NonThresholdSensor(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NonThresholdSensor::getName() {
    return "NonThresholdSensor";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NonThresholdSensor::getDescription() {
    return "Retrieving thresholds on a sensor that is not a threshold type\n"
           "as indicated by the <i>IsAccessible</i> field of the Sensor RDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NonThresholdSensor::getPrecondition() {
    return "Requires one or more non-threshold sensors which do "
           "respond with SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NonThresholdSensor::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NonThresholdSensor::runSensorTest(SaHpiSessionIdT sessionId,
                                                SaHpiRptEntryT *rptEntry,
                                                SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;

    if (sensorRec->ThresholdDefn.IsAccessible) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorThresholdsGet(sessionId,
                                                  rptEntry->ResourceId,
                                                  sensorRec->Num,
                                                  &sensorThresholds);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_INVALID_CMD) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, SENSOR_THRESHOLDS_GET,
                                 SA_ERR_HPI_INVALID_CMD, error);
        }
    }

    return status;
}

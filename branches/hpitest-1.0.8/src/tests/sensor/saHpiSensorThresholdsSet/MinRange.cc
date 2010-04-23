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

#include "MinRange.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MinRange::MinRange(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *MinRange::getName() {
    return "MinRange";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *MinRange::getDescription() {
    return "Threshold values cannot be set below the Min range as defined\n"
           "by the <i>Range</i> field of the <i>SensorDataFormat</i> of the RDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *MinRange::getPrecondition() {
    return "Requires a threshold sensor that has a minimum value for\n"
           "threshold values and supports a writeable LowCritical threshold.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT MinRange::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus MinRange::runThresholdTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;

    if (!SensorHelper::hasMin(sensorRec)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isWriteThreshold(sensorRec, SAHPI_STM_LOW_CRIT)) {
        status.assertNotSupport();
    } else {
        SaHpiSensorReadingUnionT minValue = SensorHelper::getMinValue(sensorRec);
        if (!SensorHelper::isAtLowerMachineLimit(minValue,
                                                 sensorRec->DataFormat.ReadingType)) {
            status.assertNotSupport();
        } else {
            SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

            sensorThresholds.LowCritical.IsSupported = SAHPI_TRUE;
            sensorThresholds.LowCritical.Value = 
                SensorHelper::decrement(minValue, sensorRec->DataFormat.ReadingType);

            SaErrorT error = saHpiSensorThresholdsSet(sessionId,
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
                status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET,
                                     SA_ERR_HPI_INVALID_CMD, error);
            } 
        }
    }

    return status;
}


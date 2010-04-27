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

#include "MaxRange.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MaxRange::MaxRange(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *MaxRange::getName() {
    return "MaxRange";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *MaxRange::getDescription() {
    return "Threshold values cannot be set above the Max range as defined\n"
           "by the <i>Range</i> field of the <i>SensorDataFormat</i> of the RDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *MaxRange::getPrecondition() {
    return "Requires a threshold sensor that has a maximum value for\n"
           "threshold values and supports a writeable UpCritical threshold.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT MaxRange::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus MaxRange::runThresholdTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;

    if (!SensorHelper::hasMax(sensorRec)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isWriteThreshold(sensorRec, SAHPI_STM_UP_CRIT)) {
        status.assertNotSupport();
    } else {
        SaHpiSensorReadingUnionT maxValue = SensorHelper::getMaxValue(sensorRec);
        if (SensorHelper::isAtUpperMachineLimit(maxValue, 
                                                sensorRec->DataFormat.ReadingType)) {
            status.assertNotSupport();
        } else {
            SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

            sensorThresholds.UpCritical.IsSupported = SAHPI_TRUE;
            sensorThresholds.UpCritical.Value = 
                SensorHelper::increment(maxValue, sensorRec->DataFormat.ReadingType);

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

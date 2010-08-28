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
 * Fix
 * 10/03/12 Restore the complete sensor
 *          <larswetzel@users.sourceforge.net>
 */

#include "SetUpCritical.h"
#include "Report.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetUpCritical::SetUpCritical(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetUpCritical::getName() {
    return "SetUpCritical";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetUpCritical::getDescription() {
    return "Set the <i>UpCritical</i> threshold value only.  Verify that\n"
           "the change was successful and that the <i>LowCritical</i>\n"
           "threshold was not mistakenly changed.";
}

/*****************************************************************************
 * Return the Precondition of the test case.
 *****************************************************************************/

const char *SetUpCritical::getPrecondition() {
    return "One or more threshold sensors with read/write <i>LowCritcal</i>\n"
           "and read/write <i>UpCritical</i> thresholds.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetUpCritical::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetUpCritical::runThresholdTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThresholdsT newSensorThresholds;
    SaHpiSensorThresholdsT origSensorThresholds;

    if (!SensorHelper::hasMax(sensorRec)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_LOW_CRIT)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_UP_CRIT)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorThresholdsGet(sessionId, resourceId,
                                                  sensorRec->Num, &origSensorThresholds);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
        } else {
            SaHpiSensorReadingUnionT maxValue = SensorHelper::getMaxValue(sensorRec);
            SaHpiSensorReadingUnionT minValue = SensorHelper::getMinValue(sensorRec);
            
            SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

            // NOTE: The UpCritical won't change if it is already set
            // to the maximum possible value.

            sensorThresholds.UpCritical.IsSupported = SAHPI_TRUE;
            sensorThresholds.UpCritical.Value = maxValue;

            // The low critical threshold should be ignored by the implementation.
            
            sensorThresholds.LowCritical.IsSupported = SAHPI_FALSE;
            sensorThresholds.LowCritical.Value = minValue;
            
            error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                             sensorRec->Num, &sensorThresholds);
            if (error == SA_ERR_HPI_BUSY) {
                 status.assertNotSupport();
            } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                 status.assertNotSupport();
            } else if (error != SA_OK) {
                   status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
            } else {
                error = saHpiSensorThresholdsGet(sessionId, resourceId,
                                                 sensorRec->Num, &newSensorThresholds);
                if (error == SA_ERR_HPI_BUSY) {
                    status.assertNotSupport();
                } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                    status.assertNotSupport();
                } else if (error != SA_OK) {
                    status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
                } else if (SensorHelper::compare(sensorThresholds.UpCritical, 
                                                 newSensorThresholds.UpCritical) != 0) {
                    status.assertFailure(TRACE, "UpCritical Threshold was never set.");
                } else if (SensorHelper::compare(origSensorThresholds.LowCritical,
                                                 newSensorThresholds.LowCritical) != 0) {
                    status.assertFailure(TRACE, "LowCritical Threshold was mistakenly set.");
                } else {
                    status.assertPass();
                }

                // Restore the original UpCritical Threshold
                SensorHelper::setMask(&origSensorThresholds, sensorRec->ThresholdDefn.WriteThold);
                error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                                  sensorRec->Num, &origSensorThresholds);
                status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);
            }
        }
    }

    return status;
}

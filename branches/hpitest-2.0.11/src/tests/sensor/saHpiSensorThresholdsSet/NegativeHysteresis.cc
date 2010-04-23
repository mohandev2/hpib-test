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

#include "NegativeHysteresis.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NegativeHysteresis::NegativeHysteresis(char *line)
: ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NegativeHysteresis::getName() {
    return "NegativeHysteresis";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NegativeHysteresis::getDescription() {
    return "Hysteresis values cannot be negative.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NegativeHysteresis::getPrecondition() {
    return "One or more threshold sensors with a writeable\n"
           "Pos and/or Neg Hysteresis of type integer or float.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NegativeHysteresis::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *
 * Test both Pos and Neg Hysteresis.
 *****************************************************************************/

HpiTestStatus NegativeHysteresis::runThresholdTest(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry,
                                                   SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;

    status.assertNotSupport();
    if ((SensorHelper::isWriteThreshold(sensorRec, SAHPI_STM_UP_HYSTERESIS)) && 
        (SensorHelper::isIntOrFloat(sensorRec))) {

        SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

        sensorThresholds.PosThdHysteresis.IsSupported = SAHPI_TRUE;
        if (sensorRec->DataFormat.ReadingType == SAHPI_SENSOR_READING_TYPE_INT64) {
            sensorThresholds.PosThdHysteresis.Value.SensorInt64 = -1;
        } else {
            sensorThresholds.PosThdHysteresis.Value.SensorFloat64 = -1;
        }

        SaErrorT error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                                  sensorRec->Num, &sensorThresholds);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET,
                                 SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    if ((SensorHelper::isWriteThreshold(sensorRec, SAHPI_STM_LOW_HYSTERESIS)) && 
        (SensorHelper::isIntOrFloat(sensorRec))) {
            
        SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

        sensorThresholds.NegThdHysteresis.IsSupported = SAHPI_TRUE;
        if (sensorRec->DataFormat.ReadingType == SAHPI_SENSOR_READING_TYPE_INT64) {
            sensorThresholds.NegThdHysteresis.Value.SensorInt64 = -1;
        } else {
            sensorThresholds.NegThdHysteresis.Value.SensorFloat64 = -1;
        }

        SaErrorT error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                                  sensorRec->Num, &sensorThresholds);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET,
                                 SA_ERR_HPI_INVALID_DATA, error);
        } 
    }

    return status;
}

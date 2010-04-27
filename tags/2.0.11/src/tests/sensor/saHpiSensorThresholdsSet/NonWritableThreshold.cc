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

#include "NonWritableThreshold.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NonWritableThreshold::NonWritableThreshold(char *line)
: ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NonWritableThreshold::getName() {
    return "NonWritableThreshold";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NonWritableThreshold::getDescription() {
    return "Writing to a threshold value that is not writable, i.e. read-only.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NonWritableThreshold::getPrecondition() {
    return "Requires a threshold sensor that has a read-only threshold and "
           "the sensor doesn't respond with SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NonWritableThreshold::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *
 * Find a read-only threshold.  Read that threshold and then write it back,
 * which should fail.
 *****************************************************************************/

HpiTestStatus NonWritableThreshold::runThresholdTest(SaHpiSessionIdT sessionId,
                                                     SaHpiRptEntryT *rptEntry,
                                                     SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThresholdsT newSensorThresholds;

    SaHpiSensorThdMaskT thdMask = getReadOnlyThreshold(sensorRec);
    if (thdMask == 0x0) {
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
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
        } else {
            
            SensorHelper::fill(&newSensorThresholds, sensorRec->DataFormat.ReadingType);
            SensorHelper::copy(&newSensorThresholds, &sensorThresholds, thdMask);

            error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                             sensorRec->Num, &newSensorThresholds);
            if (error == SA_ERR_HPI_BUSY) {
                status.assertNotSupport();
            } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                status.assertNotSupport();
            } else if (error == SA_ERR_HPI_INVALID_CMD) {
                status.assertPass();
            } else {
                status.assertError(TRACE, SENSOR_THRESHOLDS_GET,
                                   SA_ERR_HPI_INVALID_CMD, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Find a threshold mask of a threshold that is readable but not writable.
 * If one cannot be found, return 0x0.
 *****************************************************************************/

SaHpiSensorThdMaskT NonWritableThreshold::getReadOnlyThreshold(SaHpiSensorRecT *sensorRec) {

    SaHpiSensorThdMaskT mask[] = { SAHPI_STM_LOW_MINOR, SAHPI_STM_LOW_MAJOR,
                                   SAHPI_STM_LOW_CRIT, SAHPI_STM_UP_MINOR,
                                   SAHPI_STM_UP_MAJOR, SAHPI_STM_UP_CRIT,
                                   SAHPI_STM_UP_HYSTERESIS, SAHPI_STM_LOW_HYSTERESIS };

    for (int i = 0; i < 8; i++) {
        if (SensorHelper::isReadThreshold(sensorRec,  mask[i]) &&
            !(SensorHelper::isWriteThreshold(sensorRec, mask[i]))) {
            return mask[i];
        }
    }

    return 0x0;
}


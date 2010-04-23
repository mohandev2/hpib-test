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

#include "MultiSet.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MultiSet::MultiSet(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *MultiSet::getName() {
    return "MultiSet";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *MultiSet::getDescription() {
    return "Set multiple threshold values with one function call.\n"
           "Retrieve the thresholds to verify that the change was successful.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *MultiSet::getPrecondition() {
    return "Requires one or more threshold sensors with support for\n"
           "LowMinor, UpMinor, LowMajor, UpMajor, LowCritical, and\n"
           "UpCritical and they are all read/write.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT MultiSet::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Set the Low thresholds to the minimum possible value and set the
 * Upper thresholds to the maximum possible value.
 *****************************************************************************/

HpiTestStatus MultiSet::runThresholdTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThresholdsT newSensorThresholds;
    SaHpiSensorThresholdsT origSensorThresholds;

    if (!SensorHelper::isFullReadWrite(sensorRec)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorThresholdsGet(sessionId, rptEntry->ResourceId,
                                                  sensorRec->Num, &origSensorThresholds);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
        } else {
            SaHpiSensorReadingT maxValue = SensorHelper::getMaxReading(sensorRec);
            SaHpiSensorReadingT minValue = SensorHelper::getMinReading(sensorRec); 
            SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

            SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_LOW_CRIT, minValue);
            SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_LOW_MAJOR, minValue);
            SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_LOW_MINOR, minValue);
            SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_UP_CRIT, maxValue);
            SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_UP_MAJOR, maxValue);
            SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_UP_MINOR, maxValue);

            error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                             sensorRec->Num, &sensorThresholds);
            if (error == SA_ERR_HPI_BUSY) {
                 status.assertNotSupport();
            } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                 status.assertNotSupport();
            } else if (error != SA_OK) {
                   status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
            } else {
                error = saHpiSensorThresholdsGet(sessionId, rptEntry->ResourceId,
                                                 sensorRec->Num, &newSensorThresholds);
                if (error == SA_ERR_HPI_BUSY) {
                    status.assertNotSupport();
                } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                    status.assertNotSupport();
                } else if (error != SA_OK) {
                    status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
                } else if (!sameThresholds(&sensorThresholds, &newSensorThresholds)) {
                    status.assertFailure(TRACE, "Thresholds were never set.");
                } else {
                    status.assertPass();
                }

                // Restore the original thresholds; no need to restore hysteresis.

                origSensorThresholds.PosThdHysteresis.IsSupported = SAHPI_FALSE;
                origSensorThresholds.NegThdHysteresis.IsSupported = SAHPI_FALSE;
                error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                                 sensorRec->Num, &origSensorThresholds);//this is changed to origSensorThresholds from sensorThresholds
                status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Compare the two set of thresholds and return true if the values are the
 * same; otherwise return false.
 *****************************************************************************/

bool MultiSet::sameThresholds(SaHpiSensorThresholdsT *sensorThresholds,
                              SaHpiSensorThresholdsT *newSensorThresholds) {
    bool same = true;

    if (SensorHelper::compare(sensorThresholds->LowCritical, newSensorThresholds->LowCritical) != 0) {
        same = false;
    } else if (SensorHelper::compare(sensorThresholds->LowMajor, newSensorThresholds->LowMajor) != 0) {
        same = false;
    } else if (SensorHelper::compare(sensorThresholds->LowMinor, newSensorThresholds->LowMinor) != 0) {
        same = false;
    } else if (SensorHelper::compare(sensorThresholds->UpCritical, newSensorThresholds->UpCritical) != 0) {
        same = false;
    } else if (SensorHelper::compare(sensorThresholds->UpMajor, newSensorThresholds->UpMajor) != 0) {
        same = false;
    } if (SensorHelper::compare(sensorThresholds->UpMinor, newSensorThresholds->UpMinor) != 0) {
        same = false;
    }

    return same;
}


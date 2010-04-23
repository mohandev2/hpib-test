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

#include "Reject.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Reject::Reject() : ThresholdSensorTestCase() {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Reject::getName() {
    return "Reject";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Reject::getDescription() {
    return "Verify that the implementation rejects the entire set of\n"
           "thresholds when any one threshold is in error.  In this\n"
           "case, the <i>UpMinor</i> will be set to a valid threshold and the\n"
           "<i>PosThdHyseteresis</i> will be set to a negative value.  The\n"
           "<i>UpMinor</i> should not be set.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Reject::getPrecondition() {
    return "Requires a threshold sensor with the following:\n"
           "<ul>\n"
           "<li>Its LowMinor threshold is readable.</li>\n"
           "<li>Its UpMinor threshold is readable/writable.</li>\n"
           "<li>Its PosThdHysteresis is writable.</li>\n"
           "<li>Its data type is integer or float.</li>\n"
           "<li>Its does not respond with SA_ERR_HPI_ENTITY_NOT_PRESENT.</li>\n"
           "</ul>\n";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Reject::runThresholdTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThresholdsT newSensorThresholds;
    SaHpiSensorThresholdsT origSensorThresholds;

    if (!SensorHelper::isReadThreshold(sensorRec, SAHPI_STM_LOW_MINOR)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_UP_MINOR)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isWriteThreshold(sensorRec, SAHPI_STM_UP_HYSTERESIS)) {
        status.assertNotSupport();
    } else if (!SensorHelper::isIntOrFloat(sensorRec)) {
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

            SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

            // It is okay to set the UpMinor to the same value as LowMinor.

            SensorHelper::setThreshold(&sensorThresholds,
                                       SAHPI_STM_UP_MINOR, origSensorThresholds.LowMinor);
            sensorThresholds.UpMinor.IsSupported = SAHPI_TRUE;

            // It is invalid for the Hysteresis to be negative, which should cause a rejection.

            sensorThresholds.PosThdHysteresis.IsSupported = SAHPI_TRUE;
            if (sensorRec->DataFormat.ReadingType == SAHPI_SENSOR_READING_TYPE_INT64) {
                sensorThresholds.PosThdHysteresis.Value.SensorInt64 = -1;
            } else {
                sensorThresholds.PosThdHysteresis.Value.SensorFloat64 = -1;
            }

            error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                             sensorRec->Num, &sensorThresholds);
            if (error == SA_ERR_HPI_BUSY) {
                status.assertNotSupport();
            } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                status.assertNotSupport();
            } else if (error != SA_ERR_HPI_INVALID_DATA) {
                status.assertError(TRACE, SENSOR_THRESHOLDS_SET,
                                   SA_ERR_HPI_INVALID_DATA, error);
            } else {

                // The set has failed as it should.  Now verify that the UpMinor
                // threshold was not mistakenly modified.

                error = saHpiSensorThresholdsGet(sessionId, rptEntry->ResourceId,
                                                 sensorRec->Num, &newSensorThresholds);
                if (error == SA_ERR_HPI_BUSY) {
                    status.assertNotSupport();
                } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                    status.assertNotSupport();
                } else if (error != SA_OK) {
                      status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
                } else if (SensorHelper::compare(origSensorThresholds.UpMinor,
                                                 newSensorThresholds.UpMinor) != 0) {
                    status.assertFailure(TRACE, "UpMinor was unexpectedly modified!");
                } else {
                    status.assertPass();
                }

                // Make sure that the UpMinor has its original value.

                SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);
                SensorHelper::setThreshold(&sensorThresholds,
                                           SAHPI_STM_UP_MINOR, origSensorThresholds.UpMinor);
                sensorThresholds.UpMinor.IsSupported = SAHPI_TRUE;

                error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                                 sensorRec->Num, &sensorThresholds);
                status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);
            }
        }
    }

    return status;
}

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

#include "InOrder.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InOrder::InOrder(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InOrder::getName() {
    return "InOrder";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InOrder::getDescription() {
    return "Verify that the implementation isn't adding the thresholds one\n"
           "value at a time and thus potentially responding with an\n"
           "out-of-order error.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InOrder::getPrecondition() {
    return "Requires one or more threshold sensors where the following are true.\n"
           "<ol>\n"
           "<li>The LowMinor threshold is supported and read/write.</li>\n"
           "<li>The UpMinor threshold is supported and read/write.</li>\n"
           "<li>The LowMajor threshold is supported and read/write.</li>\n"
           "<li>The UpMajor threshold is supported and read/write.</li>\n"
           "<li>The LowCritical threshold is supported and readable.</li>\n"
           "<li>The UpCritical threshold is supported and readable.</li>"
           "</ol>";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InOrder::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * In this test, we want to verify that an implementation is correctly
 * determining in-order by looking at the group of threshold values being
 * set.  We will set the LowMajor, LowMinor, UpMinor, and UpMajor to the
 * original LowCritical and UpCritical values.  If an implementation is
 * setting the threshold values one at a time and not as a group, it might
 * mistakenly think an out-or-order is occurring.
 *****************************************************************************/

HpiTestStatus InOrder::runThresholdTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiSensorThresholdsT origSensorThresholds;

    if (!canTest(sensorRec)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorThresholdsGet(sessionId, resourceId,
                                                  sensorRec->Num,
                                                  &origSensorThresholds);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
        } else {
            error = setThresholds(sessionId, resourceId, sensorRec,
                                  origSensorThresholds.LowCritical,
                                  origSensorThresholds.LowCritical,
                                  origSensorThresholds.LowCritical,
                                  origSensorThresholds.LowCritical);

            if (error != SA_OK) {
                status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
            } else {

                // Restore the original threshold values before the next test.

                error = setThresholds(sessionId, resourceId, sensorRec,
                                      origSensorThresholds.LowMajor,
                                      origSensorThresholds.LowMinor,
                                      origSensorThresholds.UpMinor,
                                      origSensorThresholds.UpMajor);

                if (error != SA_OK) {
                    status.assertError(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
                } else {
                    error = setThresholds(sessionId, resourceId, sensorRec,
                                          origSensorThresholds.UpCritical,
                                          origSensorThresholds.UpCritical,
                                          origSensorThresholds.UpCritical,
                                          origSensorThresholds.UpCritical);

                    if (error == SA_OK) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, SENSOR_THRESHOLDS_SET, SA_OK, error);
                    }
                }

                // Restore the original threshold values.

                error = setThresholds(sessionId, resourceId, sensorRec,
                                      origSensorThresholds.LowMajor,
                                      origSensorThresholds.LowMinor,
                                      origSensorThresholds.UpMinor,
                                      origSensorThresholds.UpMajor);

                status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Can this sensor be tested?
 *****************************************************************************/

bool InOrder::canTest(SaHpiSensorRecT *sensorRec) {
    return SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_LOW_MINOR) &&
           SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_UP_MINOR) &&
           SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_LOW_MAJOR) &&
           SensorHelper::isReadWriteThreshold(sensorRec, SAHPI_STM_UP_MAJOR) &&
           SensorHelper::isReadThreshold(sensorRec, SAHPI_STM_LOW_CRIT) &&
           SensorHelper::isReadThreshold(sensorRec, SAHPI_STM_UP_CRIT);
}

/*****************************************************************************
 * Set the LowMajor, LowMinor, UpMinor, and UpMajor threshold values.
 *****************************************************************************/

SaErrorT InOrder::setThresholds(SaHpiSessionIdT sessionId,
                                SaHpiResourceIdT resourceId,
                                SaHpiSensorRecT * sensorRec,
                                SaHpiSensorReadingT &lowMajor,
                                SaHpiSensorReadingT &lowMinor,
                                SaHpiSensorReadingT &upMinor,
                                SaHpiSensorReadingT &upMajor)
{
    SaHpiSensorThresholdsT sensorThresholds;

    SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);

    SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_LOW_MAJOR, lowMajor);
    SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_LOW_MINOR, lowMinor);
    SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_UP_MINOR, upMinor);
    SensorHelper::setThreshold(&sensorThresholds, SAHPI_STM_UP_MAJOR, upMajor);

    return saHpiSensorThresholdsSet(sessionId, resourceId, sensorRec->Num, &sensorThresholds);
}


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

#include "OutOfOrder.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OutOfOrder::OutOfOrder(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OutOfOrder::getName() {
    return "OutOfOrder";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OutOfOrder::getDescription() {
    return "Thresholds are set out-of-order.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OutOfOrder::getPrecondition() {
    return "Requires a threshold sensor that has a writable threshold. "
           "Also, the sensor cannot respond with SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OutOfOrder::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *
 * This algorithm is a little tricky.  It looks for threshold pairs where the
 * lower threshold can be set to a value that is greater than the higher 
 * threshold.  That lower threshold is then set which should cause an 
 * out-of-order error.
 *****************************************************************************/

HpiTestStatus OutOfOrder::runThresholdTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThresholdsT origSensorThresholds;
    SaHpiSensorThdMaskT mask[] = { SAHPI_STM_LOW_CRIT, SAHPI_STM_LOW_MAJOR,
                                   SAHPI_STM_LOW_MINOR, SAHPI_STM_UP_MINOR,
                                   SAHPI_STM_UP_MAJOR, SAHPI_STM_UP_CRIT };

    SaErrorT error = saHpiSensorThresholdsGet(sessionId, rptEntry->ResourceId,
                                              sensorRec->Num, &origSensorThresholds);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
    } else {
        status.assertNotSupport();

        SaHpiSensorReadingT maxThreshold = SensorHelper::getMaxReading(sensorRec);

        for (int i = 0; i < 5 && !status.hasFault(); i++) {
            if (SensorHelper::isReadWriteThreshold(sensorRec, mask[i])) {
                for (int j = i+1; j < 6 && !status.hasFault(); j++) {
                    if (SensorHelper::isReadThreshold(sensorRec, mask[j])) {

                        SaHpiSensorReadingT &readThreshold = 
                                SensorHelper::getThreshold(&origSensorThresholds, mask[j]);

                        if (SensorHelper::compare(readThreshold, maxThreshold) < 0) {

                            SensorHelper::fill(&sensorThresholds, sensorRec->DataFormat.ReadingType);
                            SensorHelper::setThreshold(&sensorThresholds, mask[i], maxThreshold);
                            SensorHelper::setThreshold(&sensorThresholds, mask[j], readThreshold);
                            error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
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

                            // restore the threshold just in case it was mistakenly changed

                            error = saHpiSensorThresholdsSet(sessionId, rptEntry->ResourceId,
                                                             sensorRec->Num, &origSensorThresholds);
                            status.checkError(TRACE, SENSOR_THRESHOLDS_SET, error);

                            break; // get out of inner loop
                        }
                    }
                }
            }
        }
    }

    return status;
}

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

#include "InvalidFormat.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidFormat::InvalidFormat(char *line) : ThresholdSensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidFormat::getName() {
    return "InvalidFormat";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidFormat::getDescription() {
    return "The type of value provided for each threshold setting must\n"
           "correspond to the reading format supported by the sensor, as\n"
           "defined by the reading type in the <i>DataFormat</i> field of\n"
           "the sensor's RDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidFormat::getPrecondition() {
    return "Requires a threshold sensor for which one of the thresholds "
           "is read/write.  Also, the sensor doesn't respond with "
           "SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidFormat::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *
 * For each read/write threshold, set it's value back to the same value,
 * but change it's reading type to BUFFER which is illegal.
 *****************************************************************************/

HpiTestStatus InvalidFormat::runThresholdTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorThresholdsT sensorThresholds;
    SaHpiSensorThdMaskT mask[] = { SAHPI_STM_LOW_MINOR, SAHPI_STM_LOW_MAJOR,
                                   SAHPI_STM_LOW_CRIT, SAHPI_STM_UP_MINOR,
                                   SAHPI_STM_UP_MAJOR, SAHPI_STM_UP_CRIT,
                                   SAHPI_STM_UP_HYSTERESIS, SAHPI_STM_LOW_HYSTERESIS };

    SaErrorT error = saHpiSensorThresholdsGet(sessionId, rptEntry->ResourceId,
                                              sensorRec->Num, &sensorThresholds);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_THRESHOLDS_GET, SA_OK, error);
    } else {
        status.assertNotSupport();
        for (int i = 0; i < 8; i++) {
            if (SensorHelper::isReadWriteThreshold(sensorRec, mask[i])) {

                SensorHelper::clearSupported(&sensorThresholds);

                SaHpiSensorReadingT &reading = 
                        SensorHelper::getThreshold(&sensorThresholds, mask[i]);
                reading.IsSupported = SAHPI_TRUE;

                SaHpiSensorReadingTypeT readingType = reading.Type;
                reading.Type = SAHPI_SENSOR_READING_TYPE_BUFFER;

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
                    break;
                } 

                reading.Type = readingType;
            }
        }
    }

    return status;
}

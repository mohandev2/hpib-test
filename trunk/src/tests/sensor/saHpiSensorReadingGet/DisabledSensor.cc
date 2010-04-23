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

#include "DisabledSensor.h"

using namespace ns_saHpiSensorReadingGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DisabledSensor::DisabledSensor(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DisabledSensor::getName() {
    return "DisabledSensor";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DisabledSensor::getDescription() {
    return "The sensor is currently disabled.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DisabledSensor::getPrecondition() {
    return "Requires a sensor that is disabled or can be disabled.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DisabledSensor::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus DisabledSensor::runSensorTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiBoolT enabled;

    // Is this sensor disabled or enabled?

    SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId,
                                          sensorRec->Num, &enabled);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
    } else if (enabled && !sensorRec->EnableCtrl) {
        status.assertNotSupport();
    } else if (!enabled) {
        // run the test since the sensor is already disabled
        status.add(TRACE, testSensor(sessionId, rptEntry->ResourceId, sensorRec->Num));
    } else {

        // disable the sensor before running the test

        error = saHpiSensorEnableSet(sessionId, rptEntry->ResourceId,
                                     sensorRec->Num, SAHPI_FALSE);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_ENABLE_SET, SA_OK, error);
        } else {
            status.add(TRACE, testSensor(sessionId, rptEntry->ResourceId, sensorRec->Num));

            error = saHpiSensorEnableSet(sessionId, rptEntry->ResourceId,
                                         sensorRec->Num, SAHPI_TRUE);
            status.checkError(TRACE, SENSOR_ENABLE_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Test the disabled sensor.
 *****************************************************************************/

HpiTestStatus DisabledSensor::testSensor(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT resourceId,
                                         SaHpiSensorNumT sensorNum) {
    HpiTestStatus status;
    SaHpiSensorReadingT reading;
    SaHpiEventStateT eventState;

    SaErrorT error = saHpiSensorReadingGet(sessionId, resourceId,
                                           sensorNum, &reading, &eventState);
    if (error == SA_ERR_HPI_INVALID_REQUEST) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, SENSOR_READING_GET,
                             SA_ERR_HPI_INVALID_REQUEST, error);
    }

    return status;
}

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

#include "NullParams.h"

using namespace ns_saHpiSensorReadingGet;

/*****************************************************************************
 * Constructor 
 *****************************************************************************/

NullParams::NullParams(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullParams::getName() {
    return "NullParams";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullParams::getDescription() {
    return "The <i>Reading</i> and <i>EventState</i> parameters may be NULL.\n"
           "It is also legal for both to be NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullParams::getPrecondition() {
    return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullParams::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Try all three possible combinations of NULL pointers.
 *****************************************************************************/

HpiTestStatus NullParams::runSensorTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorReadingT reading;
    SaHpiEventStateT eventState;
    SaHpiBoolT sensorEnabled;

    SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId,
                                          sensorRec->Num, &sensorEnabled);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
    } else {
        status.add(TRACE, testSensor(sessionId, rptEntry, sensorRec,
                                     sensorEnabled, NULL, NULL));

        status.add(TRACE, testSensor(sessionId, rptEntry, sensorRec,
                                     sensorEnabled, &reading, NULL));

        status.add(TRACE, testSensor(sessionId, rptEntry, sensorRec,
                                     sensorEnabled, NULL, &eventState));
    }

    return status;
}

/*****************************************************************************
 * Test reading from a sensor depending upon the state of the sensor.
 *****************************************************************************/

HpiTestStatus NullParams::testSensor(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiSensorRecT *sensorRec,
                                     SaHpiBoolT sensorEnabled,
                                     SaHpiSensorReadingT *reading, 
                                     SaHpiEventStateT *eventState) {
    HpiTestStatus status;

    SaErrorT error = saHpiSensorReadingGet(sessionId, rptEntry->ResourceId,
                                           sensorRec->Num, reading, eventState);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (sensorEnabled && error != SA_OK) {
        status.assertFailure(TRACE, SENSOR_READING_GET, SA_OK, error);
    } else if (!sensorEnabled && error != SA_ERR_HPI_INVALID_REQUEST) {
        status.assertFailure(TRACE, SENSOR_READING_GET,
                             SA_ERR_HPI_INVALID_REQUEST, error);
    } else {
        status.assertPass();
    }

    return status;
}

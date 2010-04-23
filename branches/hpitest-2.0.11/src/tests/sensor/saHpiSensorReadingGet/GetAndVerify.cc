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

#include "GetAndVerify.h"
#include "Report.h"

using namespace ns_saHpiSensorReadingGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAndVerify::GetAndVerify(char *line) : SensorEnabledTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAndVerify::getName() {
    return "GetAndVerify";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAndVerify::getDescription() {
    return "Get the <i>Reading</i> and <i>EventState</i> and verify that\n"
           "it is valid according to the following:\n"
           "<ol>\n"
           "<li>The <i>Reading->IsSupported</i> field must match the \n"
           "    the <i>SensorRec->DataFormat.IsSupported</i> field.\n"
           "<li>The <i>Reading->Type</i> must be one of the valid enumerated values.\n"
           "<li>The returned <i>EventState</i> cannot support any event states \n"
           "    that are not supported in the sensor's RDR.\n"
           "</ol>";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAndVerify::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetAndVerify::runEnabledTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    Report report;
    SaHpiSensorReadingT reading;
    SaHpiEventStateT eventState;

    SaErrorT error = saHpiSensorReadingGet(sessionId, rptEntry->ResourceId,
                                           sensorRec->Num, &reading, &eventState);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, SENSOR_READING_GET, SA_OK, error);
    } else {
        status.add(TRACE, checkValidity(reading, eventState, sensorRec));
    }

    return status;
}

/*****************************************************************************
 * Check the Reading and EventState.
 *****************************************************************************/

HpiTestStatus GetAndVerify::checkValidity(SaHpiSensorReadingT &reading, 
                                          SaHpiEventStateT eventState,
                                          SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;

    status.assertPass();

    if (reading.IsSupported != sensorRec->DataFormat.IsSupported) {
        status.assertFailure(TRACE, "The Reading->IsSupported flag does not match\n"
                                    "the SensorRec->DataFormat.IsSupported field.");
    } else if ((reading.IsSupported) &&
               ((reading.Type > SAHPI_SENSOR_READING_TYPE_BUFFER) || 
                (reading.Type < SAHPI_SENSOR_READING_TYPE_INT64))) {
        status.assertFailure(TRACE, "Sensor Reading Type is invalid, type=%d", reading.Type);
    }

    if ((eventState & sensorRec->Events) != eventState) {
        status.assertFailure(TRACE, "Sensor is asserting events that are not supported.\n"
                                    "[Sensor RDR Events: 0x%X; Sensor EventState: 0x%X]",
                                    sensorRec->Events, eventState);
    }

    return status;
}


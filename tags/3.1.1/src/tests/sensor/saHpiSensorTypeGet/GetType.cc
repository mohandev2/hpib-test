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

#include "GetType.h"
#include "SensorHelper.h"

using namespace ns_saHpiSensorTypeGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetType::GetType(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetType::getName() {
    return "GetType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetType::getDescription() {
    return "Get the sensor type and category and verify that they are valid.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetType::getPrecondition() {
    return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetType::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetType::runSensorTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorTypeT type;
    SaHpiEventCategoryT category;

    SaErrorT error = saHpiSensorTypeGet(sessionId, rptEntry->ResourceId, 
                                        sensorRec->Num, &type, &category);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, SENSOR_TYPE_GET, SA_OK, error);
    } else {
        status.assertPass();

        if (!SensorHelper::isValidType(type)) {
            status.assertFailure(TRACE, "The returned type is invalid [0x%X]", type);
        }

        if (!SensorHelper::isValidEventCategory(category)) {
            status.assertFailure(TRACE, "The returned category is invalid [0x%X]", category);
        }
    }

    return status;
}

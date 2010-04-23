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

#include "GetEnableStatus.h"

using namespace ns_saHpiSensorEnableGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetEnableStatus::GetEnableStatus(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetEnableStatus::getName() {
    return "GetEnableStatus";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetEnableStatus::getDescription() {
    return "Get the sensor's current enable status.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetEnableStatus::getPrecondition() {
    return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetEnableStatus::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetEnableStatus::runSensorTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiBoolT sensorEnabled;

    SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId, 
                                          sensorRec->Num, &sensorEnabled);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
    }

    return status;
}

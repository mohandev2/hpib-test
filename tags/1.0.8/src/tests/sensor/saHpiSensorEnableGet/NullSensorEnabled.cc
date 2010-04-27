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

#include "NullSensorEnabled.h"

using namespace ns_saHpiSensorEnableGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullSensorEnabled::NullSensorEnabled(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullSensorEnabled::getName() {
    return "NullSensorEnabled";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullSensorEnabled::getDescription() {
    return "The <i>SensorEnabled</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullSensorEnabled::getPrecondition() {
   return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullSensorEnabled::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullSensorEnabled::runSensorTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry,
                                               SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId,
                                          sensorRec->Num, NULL);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, SENSOR_ENABLE_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

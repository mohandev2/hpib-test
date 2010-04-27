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

#include "SetEnableStatus.h"

using namespace ns_saHpiSensorEnableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetEnableStatus::SetEnableStatus(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetEnableStatus::getName() {
    return "SetEnableStatus";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetEnableStatus::getDescription() {
    return "Set the enable status for a sensor and verify the\n"
           "change was successful.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetEnableStatus::getPrecondition() {
    return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetEnableStatus::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * If the sensor's state can be changed, toggle it.
 *****************************************************************************/

HpiTestStatus SetEnableStatus::runSensorTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiBoolT sensorEnabled;

    if (!sensorRec->EnableCtrl) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId, 
                                              sensorRec->Num, &sensorEnabled);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
        } else {
            status.add(TRACE, testSensor(sessionId, rptEntry->ResourceId,
                                         sensorRec->Num, !sensorEnabled));
            if (status.isOkay()) {
                status.add(TRACE, testSensor(sessionId, rptEntry->ResourceId,
                                             sensorRec->Num, sensorEnabled));
            }

            error = saHpiSensorEnableSet(sessionId, rptEntry->ResourceId,
                                         sensorRec->Num, sensorEnabled);
            status.checkError(TRACE, SENSOR_ENABLE_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Set the sensor's enable state and verify that the change occurred.
 *****************************************************************************/

HpiTestStatus SetEnableStatus::testSensor(SaHpiSessionIdT sessionId,
                                          SaHpiResourceIdT resourceId, 
                                          SaHpiSensorNumT sensorNum,
                                          SaHpiBoolT enabled) {
    HpiTestStatus status;
    SaHpiBoolT newEnabled;

    SaErrorT error = saHpiSensorEnableSet(sessionId, resourceId, sensorNum, enabled);
    if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, SENSOR_ENABLE_SET, SA_OK, error);
    } else {
        error = saHpiSensorEnableGet(sessionId, resourceId, sensorNum, &newEnabled);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
        } else if (isBoolEqual(enabled, newEnabled)) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, 
                    "Setting sensor's enabled state to %s did not work.",
                    HpiString::boolean(enabled));
        }
    }

    return status;
}

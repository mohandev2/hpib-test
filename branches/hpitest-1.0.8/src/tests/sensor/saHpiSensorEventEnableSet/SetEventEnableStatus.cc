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

#include "SetEventEnableStatus.h"

using namespace ns_saHpiSensorEventEnableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetEventEnableStatus::SetEventEnableStatus(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetEventEnableStatus::getName() {
    return "SetEventEnableStatus";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetEventEnableStatus::getDescription() {
    return "Set the sensor's event enable status and verify the\n"
           "change was successful.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetEventEnableStatus::getPrecondition() {
    return "Requires a sensor whose EventCtrl is not SAHPI_SEC_READ_ONLY and "
           "that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetEventEnableStatus::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * If the sensor's event enable status can be changed, try toggling it.
 *****************************************************************************/

HpiTestStatus SetEventEnableStatus::runSensorTest(SaHpiSessionIdT sessionId,
                                                  SaHpiRptEntryT *rptEntry,
                                                  SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiBoolT sensorEventsEnabled;

    if (sensorRec->EventCtrl == SAHPI_SEC_READ_ONLY) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEventEnableGet(sessionId, rptEntry->ResourceId, 
                                                   sensorRec->Num, &sensorEventsEnabled);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, SENSOR_EVENT_ENABLE_GET, SA_OK, error);
        } else {
            status.add(TRACE, testSensor(sessionId, rptEntry->ResourceId,
                                         sensorRec->Num, !sensorEventsEnabled));
            if (status.isOkay()) {
                status.add(TRACE, testSensor(sessionId, rptEntry->ResourceId,
                                             sensorRec->Num, sensorEventsEnabled));
            }

            error = saHpiSensorEventEnableSet(sessionId, rptEntry->ResourceId,
                                              sensorRec->Num, sensorEventsEnabled);
            status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Set the sensor's event enable status and verify that the change occurred.
 *****************************************************************************/

HpiTestStatus SetEventEnableStatus::testSensor(SaHpiSessionIdT sessionId,
                                               SaHpiResourceIdT resourceId, 
                                               SaHpiSensorNumT sensorNum,
                                               SaHpiBoolT enabled) {
    HpiTestStatus status;
    SaHpiBoolT newEnabled;

    SaErrorT error = saHpiSensorEventEnableSet(sessionId, resourceId,
                                               sensorNum, enabled);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, SENSOR_EVENT_ENABLE_SET, SA_OK, error);
    } else {
        error = saHpiSensorEventEnableGet(sessionId, resourceId, sensorNum, &newEnabled);
        if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_ENABLE_GET, SA_OK, error);
        } else if (isBoolEqual(enabled, newEnabled)) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, 
                    "Setting sensor's event enabled state to %s did not work.",
                    HpiString::boolean(enabled));
        }
    }

    return status;
}

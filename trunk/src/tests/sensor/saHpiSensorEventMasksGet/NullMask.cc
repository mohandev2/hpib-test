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

#include "NullMask.h"

using namespace ns_saHpiSensorEventMasksGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullMask::NullMask(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullMask::getName() {
    return "NullMask";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullMask::getDescription() {
    return "The <i>AssertEventMask</i> and <i>DeassertEventMask</i> pointers "
           "may be passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullMask::getPrecondition() {
    return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullMask::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Try all three combinations of NULL pointers.
 *****************************************************************************/

HpiTestStatus NullMask::runSensorTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    status.add(TRACE, testSensor(sessionId, rptEntry, sensorRec,
                                 &assertEventMask, NULL));

    status.add(TRACE, testSensor(sessionId, rptEntry, sensorRec,
                                 NULL, &deassertEventMask));

    status.add(TRACE, testSensor(sessionId, rptEntry, sensorRec,
                                 NULL, NULL));

    return status;
}

/*****************************************************************************
 * Test a sensor by retrieving its masks.
 *****************************************************************************/

HpiTestStatus NullMask::testSensor(SaHpiSessionIdT sessionId,
                                   SaHpiRptEntryT *rptEntry,
                                   SaHpiSensorRecT *sensorRec,
                                   SaHpiEventStateT *assertEventMask,
                                   SaHpiEventStateT *deassertEventMask) {
    HpiTestStatus status;

    SaErrorT error = saHpiSensorEventMasksGet(sessionId,
                                              rptEntry->ResourceId,
                                              sensorRec->Num, 
                                              assertEventMask,
                                              deassertEventMask);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
    }

    return status;
}


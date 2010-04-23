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

#include "SensorEnabledTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorEnabledTestCase::SensorEnabledTestCase(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * The sensor must be enabled in order to run the test.
 *****************************************************************************/

HpiTestStatus SensorEnabledTestCase::runSensorTest(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry,
                                                   SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiBoolT enabled;

    SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId,
                                          sensorRec->Num, &enabled);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
    } else if (!enabled) {
        status.assertNotSupport();
    } else {
        status = runEnabledTest(sessionId, rptEntry, sensorRec);
    }

    return status;
}

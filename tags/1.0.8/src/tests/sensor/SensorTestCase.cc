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

#include "SensorTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorTestCase::SensorTestCase()
: HpiRdrTestCase(SAHPI_CAPABILITY_SENSOR) {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SensorTestCase::SensorTestCase(char *line)
: HpiRdrTestCase(line, SAHPI_CAPABILITY_SENSOR) {
}

/*****************************************************************************
 * This must be a sensor in order to perform the test.
 *****************************************************************************/

HpiTestStatus SensorTestCase::runRdrTest(SaHpiSessionIdT sessionId, 
                                         SaHpiRptEntryT *rptEntry, 
                                         SaHpiRdrT *rdr) {
    HpiTestStatus status;

    if (rdr->RdrType != SAHPI_SENSOR_RDR) {
        status.assertNotSupport();
    } else {
        pushLocation("Sensor", rdr->RdrTypeUnion.SensorRec.Num);
        status = runSensorTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.SensorRec));
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * Save the current event masks for later restoration.
 *****************************************************************************/

HpiTestStatus SensorTestCase::saveEventMasks(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId,
                                             SaHpiSensorNumT sensorNum) {
    HpiTestStatus status;

    SaErrorT error = saHpiSensorEventMasksGet(sessionId, resourceId, sensorNum, 
                                              &savedAssertEventMask, 
                                              &savedDeassertEventMask);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
    }

    return status;
}

/*****************************************************************************
 * Restore the event masks.
 *****************************************************************************/

HpiTestStatus SensorTestCase::restoreEventMasks(SaHpiSessionIdT sessionId,
                                                SaHpiResourceIdT resourceId,
                                                SaHpiSensorNumT sensorNum) {
    HpiTestStatus status;

    SaErrorT error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum,
                                              SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS,
                                              SAHPI_ALL_EVENT_STATES, 
                                              SAHPI_ALL_EVENT_STATES);
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
    } else {
        error = saHpiSensorEventMasksSet(sessionId, resourceId, sensorNum, 
                                         SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                         savedAssertEventMask, 
                                         savedDeassertEventMask);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_EVENT_MASKS_SET, SA_OK, error);
        }
    }

    return status;
}

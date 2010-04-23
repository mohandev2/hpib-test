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

#include "ReadOnly.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnly::ReadOnly(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnly::getName() {
    return "ReadOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnly::getDescription() {
    return "The sensor does not support updating the assert and deassert\n"
           "event masks (i.e., the <i>EventCtrl</i> field in the Sensor RDR\n"
           "is set to SAHPI_SEC_READ_ONLY_MASKS or SAHPI_SEC_READ_ONLY).";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires a sensor whose EventCtrl is not SAHPI_SEC_PER_EVENT and "
           "does not return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnly::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReadOnly::runSensorTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;

    if (sensorRec->EventCtrl == SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEventMasksSet(sessionId,
                                                  rptEntry->ResourceId,
                                                  sensorRec->Num, 
                                                  SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                  sensorRec->Events, 0x0);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertError(TRACE, SENSOR_EVENT_MASKS_GET,
                               SA_ERR_HPI_READ_ONLY, error);
        } 
    }

    return status;
}

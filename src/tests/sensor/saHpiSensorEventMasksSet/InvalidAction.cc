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

#include "InvalidAction.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAction::InvalidAction(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAction::getName() {
    return "InvalidAction";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAction::getDescription() {
    return "The <i>Action</i> parameter is out of range.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidAction::getPrecondition() {
    return "Requires a sensor whose EventCtrl is SAHPI_SEC_PER_EVENT and "
           "for which saHpiSensorEventMasksSet does not return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAction::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidAction::runSensorTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;

    int i = (int)SAHPI_SENS_REMOVE_EVENTS_FROM_MASKS;
    SaHpiSensorEventMaskActionT invalidAction = (SaHpiSensorEventMaskActionT)(i+1);

    if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEventMasksSet(sessionId,
                                                  rptEntry->ResourceId,
                                                  sensorRec->Num, 
                                                  invalidAction,
                                                  sensorRec->Events, 0x0); 
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, SENSOR_EVENT_MASKS_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

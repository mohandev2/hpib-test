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

#include "UnsupportedEventState.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UnsupportedEventState::UnsupportedEventState(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UnsupportedEventState::getName() {
    return "UnsupportedEventState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UnsupportedEventState::getDescription() {
    return "The <i>Action</i> parameter is set to SAHPI_SENS_ADD_EVENTS_TO_MASK,\n"
           "and either of the <i>AssertEventMask</i> or <i>DeassertEventMask</i>\n"
           "parameters includes a bit for an event state that is not supported\n"
           "by the sensor.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *UnsupportedEventState::getPrecondition() {
    return "Requires a sensor whose EventCtrl is SAHPI_SEC_PER_EVENT and "
           "which for which an unsupported event can be found in the "
           "sensor's Events bit mask.  The sensor must also not respond "
           "with SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UnsupportedEventState::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus UnsupportedEventState::runSensorTest(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry,
                                                   SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;

    if (sensorRec->EventCtrl != SAHPI_SEC_PER_EVENT) {
        status.assertNotSupport();
    } else {
        SaHpiEventStateT mask = findNonSupportedEvent(sensorRec->Events);
        if (mask == 0x0) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiSensorEventMasksSet(sessionId,
                                                      rptEntry->ResourceId,
                                                      sensorRec->Num, 
                                                      SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                      mask, 0x0); 
            if (error == SA_ERR_HPI_BUSY) {
                status.assertNotSupport();
            } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                status.assertNotSupport();
            } else if (error != SA_ERR_HPI_INVALID_DATA) {
                status.assertFailure(TRACE, SENSOR_EVENT_MASKS_SET,
                                  SA_ERR_HPI_INVALID_DATA, error);
            } else if (!hasEvtDeassertsCapability(rptEntry)) {
                mask = findNonSupportedEvent(sensorRec->Events);
                error = saHpiSensorEventMasksSet(sessionId,
                                                 rptEntry->ResourceId,
                                                 sensorRec->Num, 
                                                 SAHPI_SENS_ADD_EVENTS_TO_MASKS,
                                                 0x0, mask); 
                if (error == SA_ERR_HPI_BUSY) {
                    status.assertNotSupport();
                } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                    status.assertNotSupport();
                } else if (error == SA_ERR_HPI_INVALID_DATA) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, SENSOR_EVENT_MASKS_SET,
                                         SA_ERR_HPI_INVALID_DATA, error);
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Find a state, i.e. bit, that isn't set in the given "events".  Return 0x0
 * if all of the bits are set.
 *****************************************************************************/

SaHpiEventStateT UnsupportedEventState::findNonSupportedEvent(SaHpiEventStateT events) {
    for (int i = 0; i < 16; i++) {
        SaHpiEventStateT mask = ((SaHpiEventStateT) 1) << i;
        if (!(events & mask)) {
            return mask;
        }
    }
    return 0x0;
}

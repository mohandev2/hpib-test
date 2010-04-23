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

#include "GetMasks.h"

using namespace ns_saHpiSensorEventMasksGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetMasks::GetMasks(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetMasks::getName() {
    return "GetMasks";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetMasks::getDescription() {
    return "Get the Assert and Deassert EventMasks and verify that they\n"
           "are correct according to the sensor record's <i>Events</i> field.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetMasks::getPrecondition() {
    return "Requires a sensor that doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetMasks::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetMasks::runSensorTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;
    SaHpiEventStateT rdrEventMask = sensorRec->Events;

    SaErrorT error = saHpiSensorEventMasksGet(sessionId, 
                                              rptEntry->ResourceId, 
                                              sensorRec->Num, 
                                              &assertEventMask,
                                              &deassertEventMask); 
    if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, SENSOR_EVENT_MASKS_GET, SA_OK, error);
    } else {
        status.assertPass();
        if (!isValidMask(assertEventMask, rdrEventMask)) {
            status.assertFailure(TRACE, 
                    "The AssertEventMask [0x%X] has set bits that are "
                    "not found in SensorRec Events [0x%X].",
                    assertEventMask, rdrEventMask);
        }

        if (!isValidMask(deassertEventMask, rdrEventMask)) {
            status.assertFailure(TRACE, 
                    "The DeassertEventMask [0x%X] has set bits that are "
                    "not found in SensorRec Events [0x%X].",
                   deassertEventMask, rdrEventMask);
        }
    }

    return status;
}

/*****************************************************************************
 * Is the mask valid according the RDR event mask?
 *****************************************************************************/

bool GetMasks::isValidMask(SaHpiEventStateT mask, SaHpiEventStateT rdrEventMask) {
    return ((~rdrEventMask) & mask) == 0;
}

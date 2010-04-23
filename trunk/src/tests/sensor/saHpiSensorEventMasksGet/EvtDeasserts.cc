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

#include "EvtDeasserts.h"

using namespace ns_saHpiSensorEventMasksGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

EvtDeasserts::EvtDeasserts(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *EvtDeasserts::getName() {
    return "EvtDeasserts";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *EvtDeasserts::getDescription() {
    return "For sensors hosted by resources that have the\n"
           "SAHPI_CAPABILITY_EVT_DEASSERTS flag set in its RPT entry,\n"
           "the <i>AssertEventMask</i> and the <i>DeassertEventMask</i> values\n"
           "will always be the same.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *EvtDeasserts::getPrecondition() {
    return "Requires a sensor whose resource supports "
           "SAHPI_CAPABILITIY_EVT_DEASSERTS and for which "
           "saHpiSensorEventMasksGet() does not return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT EvtDeasserts::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus EvtDeasserts::runSensorTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry,
                                          SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiEventStateT assertEventMask;
    SaHpiEventStateT deassertEventMask;

    if (!hasEvtDeassertsCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
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
        } else if (assertEventMask == deassertEventMask) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, 
                "AssertEventMask [0x%X] is not the same as the DeassertEventMask [0x%X].",
                assertEventMask, deassertEventMask);
        }
    }

    return status;
}

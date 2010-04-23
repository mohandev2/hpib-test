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

using namespace ns_saHpiSensorEnableSet;

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
    return "The sensor does not support changing the enable status "
           "(i.e. the <i>EnableCtrl</i> field in the Sensor RDR is set to False).";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires a sensor with EnableCtrl set to false and that "
           "doesn't return SA_ERR_HPI_ENTITY_NOT_PRESENT.";
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

    if (sensorRec->EnableCtrl) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEnableSet(sessionId, rptEntry->ResourceId,
                                              sensorRec->Num, SAHPI_FALSE);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_ERR_HPI_READ_ONLY) {
            status.assertFailure(TRACE, SENSOR_ENABLE_SET, SA_ERR_HPI_READ_ONLY, error);
        } else {
            status.assertPass();
        }
    }

    return status;
}

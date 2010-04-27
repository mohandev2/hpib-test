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
 * 
 * Changes
 * 2009/05/08 - Lars.Wetzel@emerson.com
 *              return code ERR_BUSY leads to NotSupport at call SensorReadingGet
 */

#include "ReadEventStates.h"

using namespace ns_saHpiSensorEventEnableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadEventStates::ReadEventStates(char *line) : SensorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadEventStates::getName() {
    return "ReadEventStates";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadEventStates::getDescription() {
    return "Event states may still be read for a sensor even if event\n"
           "generation is disabled, by using the saHpiSensorReadingGet()\n"
           "function.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadEventStates::getPrecondition() {
    return "The following are required for running this test.\n"
           "<ol>"
           "<li>The sensor is enabled.</li>\n"
           "<li>The sensor's event enable status is disabled or "
               "can be disabled.</li>\n"
           "<li>The sensor is currently asserting one or more events\n"
               "so that the call to saHpiSensorReadingGet() returns an\n"
               "<i>EventState</i> that is non-zero.</li>"
           "</ol>";      
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadEventStates::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * We need an enabled sensor and we need to disable that's sensor's event 
 * enable status if it isn't already disabled.  We can then get the current
 * EventStates via saHpiSensorReadingGet().  But we also require that the
 * EventStates cannot be zero in order to pass this test.  The sensor must
 * be asserting an event.
 *****************************************************************************/

HpiTestStatus ReadEventStates::runSensorTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiSensorRecT *sensorRec) {
    HpiTestStatus status;
    SaHpiSensorReadingT reading;
    SaHpiEventStateT eventState;
    SaHpiBoolT sensorEnabled;
    SaHpiBoolT sensorEventsEnabled;

    if (sensorRec->EventCtrl == SAHPI_SEC_READ_ONLY) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiSensorEnableGet(sessionId, rptEntry->ResourceId,
                                              sensorRec->Num, &sensorEnabled);
        if (error == SA_ERR_HPI_BUSY) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertError(TRACE, SENSOR_ENABLE_GET, SA_OK, error);
        } else if (!sensorEnabled) {
            status.assertNotSupport();
        } else {
            error = saHpiSensorEventEnableGet(sessionId, rptEntry->ResourceId, 
                                              sensorRec->Num, &sensorEventsEnabled);
            if (error == SA_ERR_HPI_BUSY) {
                 status.assertNotSupport();
            } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                 status.assertNotSupport();
            } else if (error != SA_OK) {
                   status.assertError(TRACE, SENSOR_EVENT_ENABLE_GET, SA_OK, error);
            } else {
                if (sensorEventsEnabled) {
                    error = saHpiSensorEventEnableSet(sessionId, rptEntry->ResourceId, 
                                                      sensorRec->Num, SAHPI_FALSE);
                    status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
                }

                if (status.isOkay()) {
                    error = saHpiSensorReadingGet(sessionId, rptEntry->ResourceId,
                                                  sensorRec->Num, &reading, &eventState);
		    if (error == SA_ERR_HPI_BUSY) {
		      status.assertNotSupport();
		    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
                        status.assertNotSupport();
                    } else if (error != SA_OK) {
                        status.assertFailure(TRACE, SENSOR_READING_GET, SA_OK, error);
                    } else if (eventState == 0) {
                        status.assertNotSupport();
                    } else {
                        status.assertPass();
                    }
                }

                if (sensorEventsEnabled) {
                    error = saHpiSensorEventEnableSet(sessionId, rptEntry->ResourceId, 
                                                      sensorRec->Num, SAHPI_TRUE);
                    status.checkError(TRACE, SENSOR_EVENT_ENABLE_SET, error);
                }
            }
        }
    }

    return status;
}


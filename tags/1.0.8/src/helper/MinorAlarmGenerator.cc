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

#include "MinorAlarmGenerator.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MinorAlarmGenerator::MinorAlarmGenerator(SaHpiSessionIdT session_id) {
    this->sessionId = session_id;
    this->gen = NULL;
}

/*****************************************************************************
 * Destructor: clear the alarm that was generated.
 *****************************************************************************/

MinorAlarmGenerator::~MinorAlarmGenerator() {
    clearAlarm();
}

/*****************************************************************************
 * Generate an Minor Alarm.
 *****************************************************************************/

HpiTestStatus MinorAlarmGenerator::generateAlarm() {
    HpiTestStatus status;
    SaHpiRptEntryT rptEntry;
    SaHpiRdrT rdr;
    bool canAssert;

    SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY && gen == NULL) {
        SaHpiEntryIdT entryId = nextEntryId;
        SaErrorT error = saHpiRptEntryGet(sessionId, entryId, &nextEntryId, &rptEntry);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET, SA_OK, error);
            break;
        } else if (rptEntry.ResourceCapabilities & SAHPI_CAPABILITY_SENSOR) {
            SaHpiEntryIdT nextRdrEntryId = SAHPI_FIRST_ENTRY;
            while (!status.hasFault() && nextRdrEntryId != SAHPI_LAST_ENTRY) {
                SaHpiEntryIdT rdrEntryId = nextRdrEntryId;
                error = saHpiRdrGet(sessionId, rptEntry.ResourceId,
                                    rdrEntryId, &nextRdrEntryId, &rdr);
                if (error == SA_ERR_HPI_NOT_PRESENT) {
                    break;
                } else if (error != SA_OK) {
                    status.assertError(TRACE, RDR_GET, SA_OK, error);
                    break;
                } else if (rdr.RdrType == SAHPI_SENSOR_RDR) {
                    SaHpiSensorRecT *sensorRec = &(rdr.RdrTypeUnion.SensorRec);
                    gen = new UpMinorEventGenerator(sessionId,
                                                    rptEntry.ResourceId,
                                                    sensorRec);
                    status.add(TRACE, gen->canAssertEvent(&canAssert));
                    if (canAssert) {
                        status.add(TRACE, gen->assertEvent());
                        break;
                    } else {
                        delete gen;
                        gen = NULL;
                    }
                }
            }
        }
    }

    // we didn't find a sensor we can generate an Up Minor event
    if (gen == NULL) {
        status.assertNotSupport();
    }

    return status;
}

/*****************************************************************************
 * Clear the alarm.  
 *****************************************************************************/

HpiTestStatus MinorAlarmGenerator::clearAlarm() {
    HpiTestStatus status;

    if (gen != NULL) {
        status.add(TRACE, gen->deassertEvent());
        gen->restoreSensor();
        delete gen;
        gen = NULL;
    }

    return status;
}


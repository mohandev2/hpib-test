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

#include "AddAlarm.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddAlarm::AddAlarm(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddAlarm::getName() {
    return "AddAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddAlarm::getDescription() {
    return "Add a user alarm and then traverse the Alarm Table.\n"
           "The added user alarm must be found.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddAlarm::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddAlarm::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        status.assertPass();
        bool found = false;
        alarm.AlarmId = SAHPI_FIRST_ENTRY;
        while (true) {
            error = saHpiAlarmGetNext(sessionId,
                                      SAHPI_ALL_SEVERITIES,
                                      SAHPI_FALSE, &alarm);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertFailure(TRACE, ALARM_GET_NEXT, SA_OK, error);
                break;
            } else if (alarm.AlarmId == newAlarm.AlarmId) {
                found = true;
                break;
            }
        }

        if (!found) {
            status.assertFailure(TRACE, "Failed to find an alarm that was added.");
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

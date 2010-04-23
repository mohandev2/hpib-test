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

#include "GetAlarm.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAlarm::GetAlarm(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAlarm::getName() {
    return "GetAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAlarm::getDescription() {
    return "Retrieve an alarm and verify that it is the alarm that was requested.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAlarm::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an alarm and then retrieve it.
 *****************************************************************************/

HpiTestStatus GetAlarm::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_GET, SA_OK, error);
        } else if (alarm.AlarmId != newAlarm.AlarmId) {
            status.assertFailure(TRACE, 
                        "Expected alarm 0x%X, but retreived alarm 0x%X", 
                        newAlarm.AlarmId, alarm.AlarmId);
        } else {
            status.assertPass();
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

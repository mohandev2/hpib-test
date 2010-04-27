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

#include "IdAndTimestamp.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IdAndTimestamp::IdAndTimestamp(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IdAndTimestamp::getName() {
    return "IdAndTimestamp";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IdAndTimestamp::getDescription() {
    return "The <i>AlarmId</i> and <i>Timestamp</i> fields with the\n"
           "<i>Alarm</i> parameter are set to new values associated with\n"
           "the added alarm.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IdAndTimestamp::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Try two pairs of invalid (AlarmId, Timestamp) values for the new alarm.
 * Those values must be changed by the HPI implementation.
 *****************************************************************************/

HpiTestStatus IdAndTimestamp::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    SaHpiAlarmIdT alarmId[] = { SAHPI_ENTRY_UNSPECIFIED, (SaHpiAlarmIdT) -1 };
    SaHpiTimeT timestamp[] = { SAHPI_TIME_UNSPECIFIED, (SaHpiTimeT) -1 };

    for (int i = 0; i < 2; i++) {
        AlarmHelper::fill(&newAlarm);
        newAlarm.AlarmId = alarmId[i];
        newAlarm.Timestamp = timestamp[i];

        SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_ADD, SA_OK, error);
        } else {
            status.assertPass();

            if (newAlarm.AlarmId == alarmId[i]) {
                status.assertFailure(TRACE, 
                    "New Alarm was not assigned a new AlarmId.");
            }

            if (newAlarm.Timestamp == timestamp[i]) {
                status.assertFailure(TRACE, 
                    "New Alarm was not assigned a new Timestamp.");
            }

            // Clean up
            error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
            status.checkError(TRACE, ALARM_DELETE, error);
        }
    }

    return status;
}

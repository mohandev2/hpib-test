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

#include "DeletedAlarm.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DeletedAlarm::DeletedAlarm(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DeletedAlarm::getName() {
    return "DeletedAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DeletedAlarm::getDescription() {
    return "This function will return the next alarm meeting the specifications\n"
           "given by an HPIU User that was added to the DAT after the alarm whose\n"
           "<i>AlarmId</i> and <i>Timestamp</i> is passsed by an HPI User, even\n"
           "if the alarm associated with the <i>AlarmId</i> and <i>Timestamp</i>\n"
           "has been deleted.  In this test case, two user alarms are added to\n"
           "the DAT.  The first alarm is deleted and the attempt is made to retrieve\n"
           "the next alarm using that deleted alarm.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DeletedAlarm::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add two alarms, and then delete the first alarm.  Use that first alarm
 * for traversing the Alarm table.  We should encounter the second alarm.
 *****************************************************************************/

HpiTestStatus DeletedAlarm::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm1;
    SaHpiAlarmT newAlarm2;

    AlarmHelper::fill(&newAlarm1);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm1);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        AlarmHelper::fill(&newAlarm2);
        error = saHpiAlarmAdd(sessionId, &newAlarm2);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_ADD, SA_OK, error);

            error = saHpiAlarmDelete(sessionId, newAlarm1.AlarmId, SAHPI_MINOR);
            status.checkError(TRACE, ALARM_DELETE, error);
        } else {
            error = saHpiAlarmDelete(sessionId, newAlarm1.AlarmId, SAHPI_MINOR);
            if (error != SA_OK) {
                status.assertError(TRACE, ALARM_DELETE, SA_OK, error);
            } else {
                status.assertPass();

                bool foundAlarm2 = false;

                alarm.AlarmId = newAlarm1.AlarmId;
                alarm.Timestamp = newAlarm1.Timestamp;
                while (true) {
                    error = saHpiAlarmGetNext(sessionId,
                                               SAHPI_ALL_SEVERITIES,
                                               SAHPI_FALSE, &alarm);
                    if (error == SA_ERR_HPI_NOT_PRESENT) {
                        break;
                    } else if (error != SA_OK) {
                        status.assertError(TRACE, ALARM_GET_NEXT, SA_OK, error);
                        break;
                    } else if (alarm.AlarmId == newAlarm2.AlarmId) {
                        foundAlarm2 = true;
                    }
                }

                if (!foundAlarm2) {
                    status.assertFailure(TRACE, "Second Alarm was never found.");
                }

                error = saHpiAlarmDelete(sessionId, newAlarm2.AlarmId, SAHPI_MINOR);
                status.checkError(TRACE, ALARM_DELETE, error);
            }
        }
    }

    return status;
}

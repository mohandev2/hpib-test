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

#include "OnlyUserAlarms2.h"
#include "MinorAlarmGenerator.h"
#include "AlarmHelper.h"
#include "Report.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OnlyUserAlarms2::OnlyUserAlarms2(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OnlyUserAlarms2::getName() {
    return "OnlyUserAlarms2";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OnlyUserAlarms2::getDescription() {
    return "When deleting alarms by severity, only User Alarms\n"
           "of the requested severity will be deleted.  Use\n"
           "SAHPI_ALL_SEVERITIES to delete the alarms.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OnlyUserAlarms2::getPrecondition() {
    return "Requires a non-User Alarm in the DAT or the ability to\n"
           "generate a non-user alarm by causing an UpMinor threshold\n"
           "event on a sensor.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OnlyUserAlarms2::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Find a non-user alarm and then add a user alarm with the same severity.
 * Delete alarms by severity and verify that the user alarm was deleted, but
 * that the non-user alarm is not deleted.
 *****************************************************************************/

HpiTestStatus OnlyUserAlarms2::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    bool found;
    SaHpiAlarmT systemAlarm;

    status.add(TRACE, AlarmHelper::findSystemAlarm(sessionId, &systemAlarm, &found));
    if (status.isOkay()) {
        if (found) {
            status.add(TRACE, deleteBySeverityTest(sessionId, systemAlarm));
        } else {
            MinorAlarmGenerator alarmGenerator(sessionId);
            status.add(TRACE, alarmGenerator.generateAlarm());
            if (status.isOkay()) {
                pauseForEventProcessing();  // wait for alarm to get to DAT

                status.add(TRACE, AlarmHelper::findSystemAlarm(sessionId,
                                                               &systemAlarm,
                                                               &found));
                if (status.isOkay()) {
                    if (!found) {
                        status.assertNotSupport();
                    } else {
                        status.add(TRACE, deleteBySeverityTest(sessionId, systemAlarm));
                    }
                }
                alarmGenerator.clearAlarm();
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Add a user alarm with the same severity as a system alarm.  Then delete
 * by severity using ALL_SEVERITIES.  Only the user alarm should be deleted.
 * The system alarm should still be in the DAT.
 *****************************************************************************/

HpiTestStatus OnlyUserAlarms2::deleteBySeverityTest(SaHpiSessionIdT sessionId,
                                                    SaHpiAlarmT systemAlarm) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    SaHpiAlarmT getAlarm;

    AlarmHelper::fill(&newAlarm, systemAlarm.Severity);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmDelete(sessionId,
                                 SAHPI_ENTRY_UNSPECIFIED,
                                 SAHPI_ALL_SEVERITIES);
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_DELETE, SA_OK, error);
        } else {
            status.assertPass(); // hope all goes well

            error = saHpiAlarmGet(sessionId, systemAlarm.AlarmId, &getAlarm);
            if (error != SA_OK) {
                status.assertFailure(TRACE, ALARM_GET, SA_OK, error,
                           "The non-User Alarm was deleted.  This may be a "
                           "race condition if the alarm was cleared.");
            }

            error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &getAlarm);
            if (error == SA_OK) {
                status.assertFailure(TRACE, ALARM_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error,
                                     "The user alarm 0x%X is still in the DAT!",
                                     newAlarm.AlarmId);
            } else if (error != SA_ERR_HPI_NOT_PRESENT) {
                status.assertFailure(TRACE, ALARM_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error);
            }
        }
    }

    return status;
}


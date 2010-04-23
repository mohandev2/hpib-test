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

#include <cstdio>
#include <cstring>

#include "HpiString.h"
#include "AlarmSet.h"
#include "AlarmHelper.h"
#include "TextBufferHelper.h"
#include "HpiHelper.h"
#include "ConditionHelper.h"
#include "Report.h"

/*****************************************************************************
 * Fill the alarm with a default set of user-defined values with a
 * severity of MINOR.  This is useful when needing to create an alarm
 * to add to the Alarm table.
 *****************************************************************************/

void AlarmHelper::fill(SaHpiAlarmT *alarm) {
    fill(alarm, SAHPI_MINOR);
}

/*****************************************************************************
 * Fill the alarm with a default set of user-defined values with the
 * given severity.  This is useful when needing to create an alarm
 * to add to the Alarm table.
 *****************************************************************************/

void AlarmHelper::fill(SaHpiAlarmT *alarm, SaHpiSeverityT severity) {
    alarm->AlarmId = 0;
    alarm->Timestamp = 0;
    alarm->Severity = severity;
    alarm->Acknowledged = SAHPI_FALSE;
    ConditionHelper::fill(&alarm->AlarmCond);
}

/*****************************************************************************
 * Determine if two alarms are the same or not.
 *****************************************************************************/

bool AlarmHelper::isEqual(SaHpiAlarmT *alarm1, SaHpiAlarmT *alarm2) {
    Report report;
    return isEqual(alarm1, alarm2, report);
}

/*****************************************************************************
 * Determine if two alarms are the same or not.  Keep a record of each
 * difference in a report.
 *****************************************************************************/

bool AlarmHelper::isEqual(SaHpiAlarmT *alarm1, SaHpiAlarmT *alarm2, Report &report) {
    bool equal = true;

    if (alarm1->AlarmId != alarm2->AlarmId) {
        equal = false;
        report.add("Alarm->AlarmId fields are not equal [0x%X, 0x%X].", 
                   alarm1->AlarmId, alarm2->AlarmId);
    }

    if (alarm1->Severity != alarm2->Severity) {
        equal = false;
        report.add("Alarm->Severity fields are not equal [%s, %s].", 
                   HpiString::severity(alarm1->Severity), 
                   HpiString::severity(alarm2->Severity));
    }

    if (!ConditionHelper::isEqual(&alarm1->AlarmCond, &alarm2->AlarmCond, report)) {
        equal = false;
        report.add("Alarm->AlarmCond fields are not equal.");
    }

    return equal;
}

/*****************************************************************************
 * Is the given alarm valid or not?
 *****************************************************************************/

bool AlarmHelper::isValid(SaHpiAlarmT *alarm, Report &report) {
    bool isOkay = true;

    if (!HpiHelper::isValidSeverity(alarm->Severity)) {
        report.add("Alarm Severity (%d) is invalid.", alarm->Severity);
        isOkay = false;
    }

    if (!(alarm->AlarmCond.Type >= SAHPI_STATUS_COND_TYPE_SENSOR &&
          alarm->AlarmCond.Type <= SAHPI_STATUS_COND_TYPE_USER)) {
        report.add("AlarmCond.Type (%d) is invalid.", alarm->AlarmCond.Type);
        isOkay = false;
    }

    if (alarm->AlarmCond.Name.Length > SA_HPI_MAX_NAME_LENGTH) {
        report.add("AlarmCond.Name.Length (%d) is exceed SA_HPI_MAX_NAME_LENGTH.", 
                   alarm->AlarmCond.Name.Length);
        isOkay = false;
    }

    if (!TextBufferHelper::isValid(&alarm->AlarmCond.Data, report)) {
        isOkay = false;
    }

    return isOkay;
}

/*****************************************************************************
 * Add a full suite of alarms to the Alarm Table for testing purposes.
 * A full suite consists of the following six alarms:
 *
 *     One Acknowledged MINOR Alarm
 *     One Acknowledged MAJOR Alarm
 *     One Acknowledged CRITCAL Alarm
 *     One Unacknowledged MINOR Alarm
 *     One Unacknowledged MAJOR Alarm
 *     One Unacknowledged CRITCAL Alarm
 *
 *****************************************************************************/

HpiTestStatus AlarmHelper::addFullSuite(SaHpiSessionIdT sessionId, AlarmSet *alarmSet) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiSeverityT severity[] = { SAHPI_MINOR, SAHPI_MAJOR, SAHPI_CRITICAL,
                                  SAHPI_MINOR, SAHPI_MAJOR, SAHPI_CRITICAL };
    bool ack[] = { true, true, true, false, false, false };

    // Initialize the Alarms to be added with the three severity levels.

    alarmSet->setSize(6);
    for (int i = 0; i < 6; i++) {
        fill(&alarm, severity[i]);

        // Add the alarms and acknowledge some of them.

        SaErrorT error = saHpiAlarmAdd(sessionId, &alarm);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_ADD, SA_OK, error);
            break;
        } else {
            alarmSet->add(&alarm);
            if (ack[i]) {
                error = saHpiAlarmAcknowledge(sessionId, alarm.AlarmId, SAHPI_MINOR);
                if (error != SA_OK) {
                    status.assertError(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
                    break;
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Find a severity that is not being used in the Alarm Table by any User
 * Alarms, if possible.
 *****************************************************************************/

HpiTestStatus AlarmHelper::findUnusedSeverity(SaHpiSessionIdT sessionId, 
                                              SaHpiSeverityT *severity,
                                              bool *found) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiSeverityT testSeverity[] = { SAHPI_MINOR, SAHPI_MAJOR, SAHPI_CRITICAL };
    bool foundSeverity[] = { false, false, false };

    *found = false;
    for (int i = 0; i < 3; i++) {
        alarm.AlarmId = SAHPI_FIRST_ENTRY;
        while (true) {
            SaErrorT error = saHpiAlarmGetNext(sessionId, testSeverity[i],
                                               SAHPI_FALSE, &alarm);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, ALARM_GET_NEXT,
                                SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
                break;
            } else if (alarm.AlarmCond.Type == SAHPI_STATUS_COND_TYPE_USER) {
                foundSeverity[i] = true;
            }
        }
    }

    if (status.isOkay()) {
        for (int i = 0; i < 3; i++) {
            if (!foundSeverity[i]) {
                *found = true;
                *severity = testSeverity[i];
                break;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Find a system alarm in the Alarm Table, if possible.  A system alarm is 
 * any alarm that is not a user alarm.
 *****************************************************************************/

HpiTestStatus AlarmHelper::findSystemAlarm(SaHpiSessionIdT sessionId, SaHpiAlarmT *alarm, bool *found) {
    HpiTestStatus status;

    alarm->AlarmId = SAHPI_FIRST_ENTRY;
    *found = false;
    while (true) {
        SaErrorT error = saHpiAlarmGetNext(sessionId, SAHPI_ALL_SEVERITIES, SAHPI_FALSE, alarm);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, ALARM_GET_NEXT, SA_OK, error);
        } else if (alarm->AlarmCond.Type != SAHPI_STATUS_COND_TYPE_USER) {
            *found = true;
            break;
        }
    }

    return status;
}

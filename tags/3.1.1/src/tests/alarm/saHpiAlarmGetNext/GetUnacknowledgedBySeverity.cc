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

#include "GetUnacknowledgedBySeverity.h"
#include "AlarmHelper.h"
#include "AlarmSet.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetUnacknowledgedBySeverity::GetUnacknowledgedBySeverity(char *line)
: HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetUnacknowledgedBySeverity::getName() {
    return "GetUnacknowledgedBySeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetUnacknowledgedBySeverity::getDescription() {
    return "Traverse the Alarm Table for unacknowledged alarms of a specific\n"
           "severity. Try all three valid severities: minor, major, and critical.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetUnacknowledgedBySeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add a full suite of alarms.  Then for each severity, traverse the
 * DAT only retrieving unacknowledged alarms of a specific severity.
 *****************************************************************************/

HpiTestStatus GetUnacknowledgedBySeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    AlarmSet alarmSet;
    SaHpiSeverityT severity[] = { SAHPI_MINOR, SAHPI_MAJOR, SAHPI_CRITICAL };

    status.add(TRACE, AlarmHelper::addFullSuite(sessionId, &alarmSet));
    if (status.isOkay()) {
        for (int i = 0; i < 3; i++) {
            status.add(TRACE, traverse(sessionId, severity[i], &alarmSet));
        }
    }
    status.add(TRACE, alarmSet.deleteAll(sessionId));

    return status;
}

/*****************************************************************************
 * Traverse the DAT for unknowledged alarms with a specific severity.
 *****************************************************************************/

HpiTestStatus GetUnacknowledgedBySeverity::traverse(SaHpiSessionIdT sessionId,
                                                    SaHpiSeverityT severity,
                                                    AlarmSet *alarmSet) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    bool fatal = false;
    bool found = false;

    status.assertPass();
    alarm.AlarmId = SAHPI_FIRST_ENTRY;
    while (true) {
        SaErrorT error = saHpiAlarmGetNext(sessionId, severity,
                                           SAHPI_TRUE, &alarm);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            fatal = true;
            status.assertFailure(TRACE, ALARM_GET_NEXT, SA_OK, error);
            break;
        } else {
            if (alarm.Severity != severity) {
                status.assertFailure(TRACE, 
                        "Expected alarms with severity %s, but "
                        "saHpiAlarmGetNext() returned an invalid alarm (0x%X, %s).",
                        HpiString::severity(severity), alarm.AlarmId, 
                        HpiString::severity(alarm.Severity));
            } 

            if (alarm.Acknowledged) {
                status.assertFailure(TRACE, "Retrieved an acknowledged alarm.  "
                                         "Alarm Id: 0x%X", alarm.AlarmId);
            } 

            if (alarmSet->contains(alarm.AlarmId)) {
                found = true;
            }
        }
    }

    if (!found && !fatal) {
        status.assertFailure(TRACE, 
                "Failed to get a user alarm that was added with the severity %s", 
                HpiString::severity(severity));
    }

    return status;
}

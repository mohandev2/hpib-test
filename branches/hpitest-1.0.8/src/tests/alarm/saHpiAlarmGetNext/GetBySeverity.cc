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

#include "GetBySeverity.h"
#include "AlarmHelper.h"
#include "AlarmSet.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetBySeverity::GetBySeverity(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetBySeverity::getName() {
    return "GetBySeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetBySeverity::getDescription() {
    return "Traverse the Alarm Table for alarms with a specific severity.\n"
           "Try all three valid severities: minor, major, and critical.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetBySeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add alarms for of each severity and then traverse the DAT for
 * each severity.
 *****************************************************************************/

HpiTestStatus GetBySeverity::runSessionTest(SaHpiSessionIdT sessionId) {
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
 * Traverse the DAT for a specific severity.
 *****************************************************************************/

HpiTestStatus GetBySeverity::traverse(SaHpiSessionIdT sessionId,
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
                                           SAHPI_FALSE, &alarm);
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
            
            if (alarmSet->contains(alarm.AlarmId)) {
                found = true;
            }
        }
    }

    if (!found && !fatal) {
        status.assertFailure(TRACE, 
                "Failed to retrieve a user alarm that was added with a severity of %s", 
                HpiString::severity(severity));
    }

    return status;
}

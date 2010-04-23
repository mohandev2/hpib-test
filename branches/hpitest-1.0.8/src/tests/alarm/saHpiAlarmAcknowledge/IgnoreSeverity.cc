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

#include "IgnoreSeverity.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IgnoreSeverity::IgnoreSeverity(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getName() {
    return "IgnoreSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getDescription() {
    return "If the <i>AlarmId</i> parameter has a value other than\n"
           "SAHPI_ENTRY_UNSPECIFIED, the <i>Severity</i> paramter is ignored.\n"
           "An invalid severity will be used in order to verify that it is\n"
           "ignored.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IgnoreSeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus IgnoreSeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm, SAHPI_MINOR);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmAcknowledge(sessionId, 
                                      newAlarm.AlarmId,
                                      (SaHpiSeverityT) (SAHPI_OK + 1));
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
        } else {
            error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
            if (error != SA_OK) {
                status.assertError(TRACE, ALARM_GET, SA_OK, error);
            } else if (alarm.Acknowledged) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                        "Failed to acknowlede a specific Alarm (SAHPI_MINOR) while "
                        "passing in a different severity of (SAHPI_OK + 1).");
           }
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

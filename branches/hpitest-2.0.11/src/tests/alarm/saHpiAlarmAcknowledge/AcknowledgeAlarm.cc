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

#include "AcknowledgeAlarm.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeAlarm::AcknowledgeAlarm(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeAlarm::getName() {
    return "AcknowledgeAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeAlarm::getDescription() {
    return "Acknowledge a specific alarm.  Verify that the\n"
           "<i>Alarm->Acknowledged</i> field is set to True.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeAlarm::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AcknowledgeAlarm::runSessionTest(SaHpiSessionIdT sessionId) {
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
            status.assertError(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
        } else if (alarm.Acknowledged) {
            status.assertError(TRACE, "The newly added Alarm is already acknowledged!");
        } else {
            error = saHpiAlarmAcknowledge(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
            if (error != SA_OK) {
                status.assertFailure(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
            } else {
                error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
                if (error != SA_OK) {
                    status.assertError(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
                } else if (!alarm.Acknowledged) {
                    status.assertFailure(TRACE, "Alarm was acknowledged, but "
                                                "Acknowledged field is still False.");
                } else {
                    status.assertPass();
                }
            }
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

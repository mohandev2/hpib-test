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

#include "UnacknowledgedOnly.h"
#include "AlarmHelper.h"
#include "AlarmSet.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UnacknowledgedOnly::UnacknowledgedOnly(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UnacknowledgedOnly::getName() {
    return "UnacknowledgedOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UnacknowledgedOnly::getDescription() {
    return "Traverse the Alarm Table for unacknowledged alarms only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UnacknowledgedOnly::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.  
 *
 * Some acknowledged and unacknowledge alarms are added the to the Alarm
 * Table.  We should only find the unacknowledged alarms.
 *****************************************************************************/

HpiTestStatus UnacknowledgedOnly::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    AlarmSet alarmSet;
    SaHpiAlarmT alarm;
    bool fatal = false;

    status.add(TRACE, AlarmHelper::addFullSuite(sessionId, &alarmSet));
    if (status.isOkay()) {

        status.assertPass();

        int count = 0;
        alarm.AlarmId = SAHPI_FIRST_ENTRY;
        while (true) {
            SaErrorT error = saHpiAlarmGetNext(sessionId, SAHPI_ALL_SEVERITIES,
                                               SAHPI_TRUE, &alarm);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                fatal = true;
                status.assertFailure(TRACE, ALARM_GET_NEXT, SA_OK, error);
                break;
            } else {
                if (alarm.Acknowledged) {
                    status.assertFailure(TRACE, 
                            "Expected unacknowledged alarms only, but saHpiAlarmGetNext()\n"
                            "returned an acknowledged alarm (0x%X).", alarm.AlarmId);
                } 
                
                if (alarmSet.contains(alarm.AlarmId)) {
                    count++;
                }
            }
        }

        if (count != 3 && !fatal) {
            status.assertFailure(TRACE, "Failed to get all of the user alarms "
                                        "that were added and unacknowledged.");
        }
    }

    status.add(TRACE, alarmSet.deleteAll(sessionId));

    return status;
}

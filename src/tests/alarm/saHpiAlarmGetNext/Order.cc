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

#include "Order.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Order::Order(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Order::getName() {
    return "Order";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Order::getDescription() {
    return "All alarms contained in the DAT are maintained in the order\n"
           "in which they were added.  Two user alarms will be added and\n"
           "it is expected that they will be retrieved in the same order\n"
           "in which they were added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Order::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add two alarms and make sure we retrieve them in the same order they
 * were added.
 *****************************************************************************/

HpiTestStatus Order::runSessionTest(SaHpiSessionIdT sessionId) {
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
        } else {

            bool foundAlarm1 = false;
            bool foundAlarm2 = false;

            status.assertPass();

            alarm.AlarmId = SAHPI_FIRST_ENTRY;
            while (true) {
                error = saHpiAlarmGetNext(sessionId,
                                           SAHPI_ALL_SEVERITIES,
                                           SAHPI_FALSE, &alarm);
                if (error == SA_ERR_HPI_NOT_PRESENT) {
                    break;
                } else if (error != SA_OK) {
                    status.assertError(TRACE, ALARM_GET_NEXT, SA_OK, error);
                    break;
                } else if (alarm.AlarmId == newAlarm1.AlarmId) {
                    foundAlarm1 = true;
                } else if (alarm.AlarmId == newAlarm2.AlarmId) {
                    foundAlarm2 = true;
                    if (!foundAlarm1) {
                        status.assertFailure(TRACE, 
                                        "Alarm 2 was found before Alarm 1.");
                        break;
                    }
                }
            }

            if (!foundAlarm2) {
                status.assertFailure(TRACE, "Alarm 2 was never found.");
            }

            error = saHpiAlarmDelete(sessionId, newAlarm2.AlarmId, SAHPI_MINOR);
            status.checkError(TRACE, ALARM_DELETE, error);
        }

        error = saHpiAlarmDelete(sessionId, newAlarm1.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

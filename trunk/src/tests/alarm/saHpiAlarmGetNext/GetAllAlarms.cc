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

#include "GetAllAlarms.h"
#include "AlarmHelper.h"
#include "AlarmSet.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAllAlarms::GetAllAlarms(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAllAlarms::getName() {
    return "GetAllAlarms";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAllAlarms::getDescription() {
    return "A full suite of alarms are added with different severities\n"
           "and acknowledgements.  The Alarm table is then traversed.\n"
           "All of the added alarms must be found and the AlarmIds must\n"
           "be valid, i.e not SAHPI_FIRST_ENTRY or SAHPI_LAST_ENTRY.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAllAlarms::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetAllAlarms::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    AlarmSet alarmSet;
    SaHpiAlarmT alarm;

    status.add(TRACE, AlarmHelper::addFullSuite(sessionId, &alarmSet));
    if (status.isOkay()) {
        int count = 0;
        alarm.AlarmId = SAHPI_FIRST_ENTRY;
        while (true) {
            SaErrorT error = saHpiAlarmGetNext(sessionId, SAHPI_ALL_SEVERITIES,
                                               SAHPI_FALSE, &alarm);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertFailure(TRACE, ALARM_GET_NEXT, SA_OK, error);
                break;
            } else if (alarm.AlarmId == SAHPI_FIRST_ENTRY) {
                status.assertFailure(TRACE, 
                        "Alarm was returned with an Id of SAHPI_FIRST_ENTRY.");
            } else if (alarm.AlarmId == SAHPI_LAST_ENTRY) {
                status.assertFailure(TRACE, 
                        "Alarm was returned with an Id of SAHPI_LAST_ENTRY.");
            } else if (alarmSet.contains(alarm.AlarmId)) {
                count++;
            }
        }

        if (status.isOkay()) {
            if (count == 6) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                    "Failed to get all of the user alarms that were added.");
            }
        }
    }

    status.add(TRACE, alarmSet.deleteAll(sessionId));

    return status;
}

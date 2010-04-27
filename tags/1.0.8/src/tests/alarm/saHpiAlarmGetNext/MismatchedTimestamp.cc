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

#include "MismatchedTimestamp.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MismatchedTimestamp::MismatchedTimestamp(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *MismatchedTimestamp::getName() {
    return "MismatchedTimestamp";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *MismatchedTimestamp::getDescription() {
    return "The <i>Alarm->AlarmId</i> matches an alarm in the DAT, but the passed\n"
           "<i>Alarm->Timestamp</i> does not match the timestamp of that alarm.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT MismatchedTimestamp::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus MismatchedTimestamp::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        alarm.AlarmId = newAlarm.AlarmId;
        alarm.Timestamp = newAlarm.Timestamp + 1;

        error = saHpiAlarmGetNext(sessionId,
                                  SAHPI_ALL_SEVERITIES,
                                  SAHPI_FALSE, &alarm);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ALARM_GET_NEXT,
                                 SA_ERR_HPI_INVALID_DATA, error);
        }

        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

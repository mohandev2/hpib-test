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

#include "DeleteAllUserAlarms.h"
#include "AlarmHelper.h"
#include "AlarmSet.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DeleteAllUserAlarms::DeleteAllUserAlarms(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DeleteAllUserAlarms::getName() {
    return "DeleteAllUserAlarms";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DeleteAllUserAlarms::getDescription() {
    return "To delete all User Alarms contained within the DAT, set\n"
           "the <i>AlarmId</i> parameter to SAHPI_ENTRY_UNSPECIFIED and\n"
           "the <i>Severity</i> parameter to SAHPI_ALL_SEVERITIES.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DeleteAllUserAlarms::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add user alarms, one for each severity, and then delete all of the
 * user alarms.  Verify that those user alarms were in fact deleted.
 *****************************************************************************/

HpiTestStatus DeleteAllUserAlarms::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT getAlarm;
    AlarmSet alarmSet(3);

    status.add(TRACE, addAlarms(sessionId, &alarmSet));
    if (!status.isOkay()) {
        status.add(TRACE, alarmSet.deleteAll(sessionId));
    } else {
        SaErrorT error = saHpiAlarmDelete(sessionId,
                                          SAHPI_ENTRY_UNSPECIFIED,
                                          SAHPI_ALL_SEVERITIES);
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_DELETE, SA_OK, error);
        } else {
            for (int i = 0; i < alarmSet.getSize(); i++) {
                SaHpiAlarmT *alarm = alarmSet.getAlarm(i);
                error = saHpiAlarmGet(sessionId, alarm->AlarmId, &getAlarm);
                if (error == SA_OK) {
                    status.assertFailure(TRACE, ALARM_GET,
                                         SA_ERR_HPI_NOT_PRESENT, error,
                                         "Failed to delete a User Alarm of severity %s.",
                                         HpiString::severity(alarm->Severity));
                } else if (error == SA_ERR_HPI_NOT_PRESENT) {
                    status.assertPass();
                } else {
                    status.assertError(TRACE, ALARM_GET,
                                       SA_ERR_HPI_NOT_PRESENT, error);
                }
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Add three new alarms, one for each severity, and return those
 * alarms in the "alarmSet".
 *****************************************************************************/

HpiTestStatus DeleteAllUserAlarms::addAlarms(SaHpiSessionIdT sessionId,
                                             AlarmSet *alarmSet) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    SaHpiSeverityT severity[] = { SAHPI_MINOR, SAHPI_MAJOR, SAHPI_CRITICAL };

    for (int i = 0; i < 3; i++) {
        AlarmHelper::fill(&newAlarm, severity[i]);
        SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
        if (error == SA_OK) {
            alarmSet->add(&newAlarm);
        } else {
            status.assertError(TRACE, ALARM_ADD, SA_OK, error);
            break;
        }
    }

    return status;
}

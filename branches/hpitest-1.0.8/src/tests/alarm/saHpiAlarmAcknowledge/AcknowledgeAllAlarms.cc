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

#include "AcknowledgeAllAlarms.h"
#include "AlarmHelper.h"
#include "AlarmSet.h"

using namespace ns_saHpiAlarmAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeAllAlarms::AcknowledgeAllAlarms(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeAllAlarms::getName() {
    return "AcknowledgeAllAlarms";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeAllAlarms::getDescription() {
    return "Acknowledge all alarms in the DAT, regardless of severity.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeAllAlarms::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Acknowledge all alarms, regardless of severity.  
 *****************************************************************************/

HpiTestStatus AcknowledgeAllAlarms::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT getAlarm;
    AlarmSet alarmSet(3);

    status.add(TRACE, addAlarms(sessionId, &alarmSet));
    if (status.isOkay()) {
        SaErrorT error = saHpiAlarmAcknowledge(sessionId,
                                               SAHPI_ENTRY_UNSPECIFIED,
                                               SAHPI_ALL_SEVERITIES);
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
        } else {
            for (int i = 0; i < 3; i++) {
                SaHpiAlarmT *alarm = alarmSet.getAlarm(i);
                error = saHpiAlarmGet(sessionId, alarm->AlarmId, &getAlarm);
                if (error != SA_OK) {
                    status.assertError(TRACE, ALARM_GET, SA_OK, error);
                    break;
                } else if (getAlarm.Acknowledged) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, 
                            "Alarm of severity %s was not acknowledged.",
                            HpiString::severity(alarm->Severity));
                } 
            }
        }
    }
    status.add(TRACE, alarmSet.deleteAll(sessionId));

    return status;
}

/*****************************************************************************
 * Add three alarms with different severities to the DAT.
 *****************************************************************************/

HpiTestStatus AcknowledgeAllAlarms::addAlarms(SaHpiSessionIdT sessionId,
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

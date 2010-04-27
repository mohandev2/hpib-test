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

#include "AcknowledgeBySeverity.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeBySeverity::AcknowledgeBySeverity(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeBySeverity::getName() {
    return "AcknowledgeBySeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeBySeverity::getDescription() {
    return "To acknowledge all alarms of a specific severity contained with\n"
           "the DAT, set the <i>Severity</i> parameter to the appropriate value,\n"
           "and set the <i>AlarmId</i> parameter to SAHPI_ENTRY_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeBySeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Test all combinations of acknowledging severities.
 *****************************************************************************/

HpiTestStatus AcknowledgeBySeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    status.add(TRACE, runAckTest(sessionId, SAHPI_MINOR, SAHPI_MAJOR));
    status.add(TRACE, runAckTest(sessionId, SAHPI_MINOR, SAHPI_CRITICAL));

    status.add(TRACE, runAckTest(sessionId, SAHPI_MAJOR, SAHPI_MINOR));
    status.add(TRACE, runAckTest(sessionId, SAHPI_MAJOR, SAHPI_CRITICAL));

    status.add(TRACE, runAckTest(sessionId, SAHPI_CRITICAL, SAHPI_MINOR));
    status.add(TRACE, runAckTest(sessionId, SAHPI_CRITICAL, SAHPI_MAJOR));

    return status;
}

/*****************************************************************************
 * In this test, add two alarms with the "ackSeverity" and one alarm with
 * the "unackSeverity".  Then acknowledge alarms in the DAT with a severity of 
 * "ackSeverity".  Verify that both of the "ackSeverity" alarms were 
 * acknowledged and that the alarm with the "unackSeverity" was not 
 * acknowledged.
 *****************************************************************************/

HpiTestStatus AcknowledgeBySeverity::runAckTest(SaHpiSessionIdT sessionId,
                                                SaHpiSeverityT ackSeverity,
                                                SaHpiSeverityT unackSeverity) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAckAlarm1;
    SaHpiAlarmT newAckAlarm2;
    SaHpiAlarmT newUnackAlarm;

    AlarmHelper::fill(&newAckAlarm1, ackSeverity);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAckAlarm1);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        AlarmHelper::fill(&newAckAlarm2, ackSeverity);
        error = saHpiAlarmAdd(sessionId, &newAckAlarm2);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_ADD, SA_OK, error);
        } else {
            AlarmHelper::fill(&newUnackAlarm, unackSeverity);
            error = saHpiAlarmAdd(sessionId, &newUnackAlarm);
            if (error != SA_OK) {
                status.assertError(TRACE, ALARM_ADD, SA_OK, error);
            } else {
                error = saHpiAlarmAcknowledge(sessionId,
                                              SAHPI_ENTRY_UNSPECIFIED,
                                              ackSeverity);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error);
                } else {
                    status.assertPass();

                    error = saHpiAlarmGet(sessionId, newAckAlarm1.AlarmId, &alarm);
                    if (error != SA_OK) {
                        status.assertError(TRACE, ALARM_GET, SA_OK, error);
                    } else if (!alarm.Acknowledged) {
                        status.assertFailure(TRACE,
                            "Failed to acknowledge an alarm with a severity of %s.",
                            HpiString::severity(ackSeverity));
                    } 

                    error = saHpiAlarmGet(sessionId, newAckAlarm2.AlarmId, &alarm);
                    if (error != SA_OK) {
                        status.assertError(TRACE, ALARM_GET, SA_OK, error);
                    } else if (!alarm.Acknowledged) {
                        status.assertFailure(TRACE, 
                            "Failed to acknowledge an alarm with a severity of %s.",
                            HpiString::severity(ackSeverity));
                    } 

                    error = saHpiAlarmGet(sessionId, newUnackAlarm.AlarmId, &alarm);
                    if (error != SA_OK) {
                        status.assertError(TRACE, ALARM_GET, SA_OK, error);
                    } else if (alarm.Acknowledged) {
                        status.assertFailure(TRACE, 
                            "Acknowledged alarms with a severity of %s, but\n"
                            "incorrectly acknowledged an alarm with a severity of %s.",
                            HpiString::severity(ackSeverity),
                            HpiString::severity(unackSeverity));
                    } 
                }

                error = saHpiAlarmDelete(sessionId, newUnackAlarm.AlarmId, SAHPI_MINOR);
                status.checkError(TRACE, ALARM_DELETE, error);
            }

            error = saHpiAlarmDelete(sessionId, newAckAlarm2.AlarmId, SAHPI_MINOR);
            status.checkError(TRACE, ALARM_DELETE, error);
        }

        error = saHpiAlarmDelete(sessionId, newAckAlarm1.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

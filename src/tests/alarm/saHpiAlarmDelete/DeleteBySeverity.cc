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

#include "DeleteBySeverity.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DeleteBySeverity::DeleteBySeverity(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DeleteBySeverity::getName() {
    return "DeleteBySeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DeleteBySeverity::getDescription() {
    return "To delete a group of User Alarms, set the <i>AlarmId</i> parameter\n"
           "to SAHPI_ENTRY_UNSPECIFIED, and set the <i>Severity</i> parameter\n"
           "to identify which severity of alarms should be deleted.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DeleteBySeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Test all combinations of acknowledging severities.
 *****************************************************************************/

HpiTestStatus DeleteBySeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    status.add(TRACE, runDeleteTest(sessionId, SAHPI_MINOR, SAHPI_MAJOR));
    status.add(TRACE, runDeleteTest(sessionId, SAHPI_MINOR, SAHPI_CRITICAL));

    status.add(TRACE, runDeleteTest(sessionId, SAHPI_MAJOR, SAHPI_MINOR));
    status.add(TRACE, runDeleteTest(sessionId, SAHPI_MAJOR, SAHPI_CRITICAL));

    status.add(TRACE, runDeleteTest(sessionId, SAHPI_CRITICAL, SAHPI_MINOR));
    status.add(TRACE, runDeleteTest(sessionId, SAHPI_CRITICAL, SAHPI_MAJOR));

    return status;
}

/*****************************************************************************
 * In this test, add two alarms with the "deleteSeverity" and one alarm with
 * "severity".  Then delete alarms in the DAT with a severity of 
 * "deleteSeverity".  Verify that both of the "deleteSeverity" alarms were 
 * deleted and that the alarm with the "severity" was not deleted.
 *****************************************************************************/

HpiTestStatus DeleteBySeverity::runDeleteTest(SaHpiSessionIdT sessionId,
                                              SaHpiSeverityT  deleteSeverity,
                                              SaHpiSeverityT  severity) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newDeleteAlarm1;
    SaHpiAlarmT newDeleteAlarm2;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newDeleteAlarm1, deleteSeverity);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newDeleteAlarm1);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        AlarmHelper::fill(&newDeleteAlarm2, deleteSeverity);
        error = saHpiAlarmAdd(sessionId, &newDeleteAlarm2);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_ADD, SA_OK, error);
        } else {
            AlarmHelper::fill(&newAlarm, severity);
            error = saHpiAlarmAdd(sessionId, &newAlarm);
            if (error != SA_OK) {
                status.assertError(TRACE, ALARM_ADD, SA_OK, error);
            } else {
                error = saHpiAlarmDelete(sessionId, SAHPI_ENTRY_UNSPECIFIED, deleteSeverity);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, ALARM_DELETE, SA_OK, error);
                } else {
                    status.assertPass();

                    error = saHpiAlarmGet(sessionId, newDeleteAlarm1.AlarmId, &alarm);
                    if (error != SA_ERR_HPI_NOT_PRESENT) {
                        status.assertError(TRACE, ALARM_GET, SA_ERR_HPI_NOT_PRESENT, error,
                                           "User Alarm with severity %s was not deleted as expected.", 
                                           HpiString::severity(deleteSeverity));
                    } 

                    error = saHpiAlarmGet(sessionId, newDeleteAlarm2.AlarmId, &alarm);
                    if (error != SA_ERR_HPI_NOT_PRESENT) {
                        status.assertError(TRACE, ALARM_GET, SA_ERR_HPI_NOT_PRESENT, error,
                                           "User Alarm with severity %s was not deleted as expected.", 
                                           HpiString::severity(deleteSeverity));
                    } 

                    error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
                    if (error != SA_OK) {
                        status.assertError(TRACE, ALARM_GET, SA_OK, error,
                                           "User Alarm with severity %s was unexpectedly deleted.", 
                                           HpiString::severity(severity));
                    } 
                }

                error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
                status.checkError(TRACE, ALARM_DELETE, error);
            }
        }
    }

    return status;
}


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

#include "AddAlarm.h"
#include "AlarmHelper.h"
#include "Report.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddAlarm::AddAlarm(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddAlarm::getName() {
    return "AddAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddAlarm::getDescription() {
    return "Add an new alarm to the DAT.  Verify the addition by\n"
           "retrieving the new alarm and examining its fields.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddAlarm::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddAlarm::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;
    Report report;

    AlarmHelper::fill(&newAlarm);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertFailure(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_GET, SA_OK, error);
        } else if (!AlarmHelper::isValid(&alarm, report)) {
            status.assertFailure(TRACE, report.toString());
        } else if (!AlarmHelper::isEqual(&newAlarm, &alarm, report)) {
            char buf[2000];
            sprintf(buf,
                "The alarm that was retrieved doesn't match the alarm that was added.\n%s", 
                report.toString());
            status.assertFailure(TRACE, buf);
        } else {
            status.assertPass();
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

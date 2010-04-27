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

#include "IgnoreSeverity.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IgnoreSeverity::IgnoreSeverity(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getName() {
    return "IgnoreSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getDescription() {
    return "The <i>Severity</i> parameter is ignored when the <i>AlarmId</i>\n"
           "parameter is set to a value other than SAHP_ENTRY_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IgnoreSeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus IgnoreSeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm, SAHPI_MINOR);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId,
                                 (SaHpiSeverityT) (SAHPI_OK + 1));
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_DELETE, SA_OK, error);
        } else {
            error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ALARM_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error,
                                     "Alarm 0x%X was not deleted!",
                                     newAlarm.AlarmId);
            }
        }
    }

    return status;
}

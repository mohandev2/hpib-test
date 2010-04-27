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

#include "NonEmptyAlarmTestCase.h"
#include "AlarmHelper.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NonEmptyAlarmTestCase::NonEmptyAlarmTestCase(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * If the Alarm Table is empty, add a user alarm before running
 * the main test.
 *****************************************************************************/

HpiTestStatus NonEmptyAlarmTestCase::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiDomainInfoT info;

    SaErrorT error = saHpiDomainInfoGet(sessionId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, DOMAIN_INFO_GET, SA_OK, error);
    } else if (info.ActiveAlarms > 0) {
        status.add(TRACE, runAlarmTest(sessionId));
    } else {
        SaHpiAlarmT alarm;
        AlarmHelper::fill(&alarm);
        error = saHpiAlarmAdd(sessionId, &alarm);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_ADD, SA_OK, error);
        } else {
            status.add(TRACE, runAlarmTest(sessionId));

            error = saHpiAlarmDelete(sessionId, alarm.AlarmId, SAHPI_MINOR);
            status.checkError(TRACE, ALARM_DELETE, error);
        }
    }

    return status;
}

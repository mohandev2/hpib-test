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

#include "InvalidAlarmId.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAlarmId::InvalidAlarmId(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAlarmId::getName() {
    return "InvalidAlarmId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAlarmId::getDescription() {
    return "The requested <i>AlarmId</i> does not correspond to\n"
           "an alarm contained in the DAT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAlarmId::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *
 * To get an AlarmId that doesn't correspond to any alarm, add a new
 * alarm, delete it, and then use it's AlarmId.
 *****************************************************************************/

HpiTestStatus InvalidAlarmId::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        if (error != SA_OK) {
            status.assertError(TRACE, ALARM_DELETE, SA_OK, error);
        } else {
            error = saHpiAlarmGet(sessionId, newAlarm.AlarmId, &alarm);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ALARM_GET,
                                     SA_ERR_HPI_NOT_PRESENT, error);
            }
        }
    }

    return status;
}

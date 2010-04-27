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

#include "InvalidAlarmCond.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAlarmCond::InvalidAlarmCond(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAlarmCond::getName() {
    return "InvalidAlarmCond";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAlarmCond::getDescription() {
    return "The <i>Alarm->AlarmCond.Type</i> is not SAHPI_STATUS_COND_TYPE_USER.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAlarmCond::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * Try all of the invalid Status Cond Type values.
 *****************************************************************************/

HpiTestStatus InvalidAlarmCond::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    SaHpiStatusCondTypeT condType[] = { SAHPI_STATUS_COND_TYPE_SENSOR,
                                        SAHPI_STATUS_COND_TYPE_RESOURCE,
                                        SAHPI_STATUS_COND_TYPE_OEM };

    for (int i = 0; i < 3; i++) {
        AlarmHelper::fill(&newAlarm);
        newAlarm.AlarmCond.Type = condType[i];
        SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
        if (error == SA_ERR_HPI_INVALID_PARAMS) 
        {
            status.assertPass();
        } else 
        {
            status.assertFailure(TRACE, ALARM_ADD,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Alarm->AlarmCond.Type: %s",
                                 HpiString::statusCondType(condType[i]));
        }
    }

    return status;
}

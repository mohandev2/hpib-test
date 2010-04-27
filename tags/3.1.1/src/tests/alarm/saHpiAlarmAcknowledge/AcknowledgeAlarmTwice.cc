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

#include "AcknowledgeAlarmTwice.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeAlarmTwice::AcknowledgeAlarmTwice(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeAlarmTwice::getName() {
    return "AcknowledgeAlarmTwice";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeAlarmTwice::getDescription() {
    return "If an alarm has been previously acknowledged, acknowledging\n"
           "it again has no effect.  However, this is not an error.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeAlarmTwice::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an alarm and acknowledge it twice.
 *****************************************************************************/

HpiTestStatus AcknowledgeAlarmTwice::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;

    AlarmHelper::fill(&newAlarm);
    SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
    if (error != SA_OK) {
        status.assertError(TRACE, ALARM_ADD, SA_OK, error);
    } else {
        error = saHpiAlarmAcknowledge(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        if (error != SA_OK) {
            status.assertFailure(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error,
                              "Failed to originally acknowledge the new alarm.");
        } else {
            error = saHpiAlarmAcknowledge(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
            if (error == SA_OK) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ALARM_ACKNOWLEDGE, SA_OK, error,
                                  "Failed to acknowledge the same alarm twice.");
            }
        }

        // Clean up
        error = saHpiAlarmDelete(sessionId, newAlarm.AlarmId, SAHPI_MINOR);
        status.checkError(TRACE, ALARM_DELETE, error);
    }

    return status;
}

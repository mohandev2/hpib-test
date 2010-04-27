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

#include "NonUserAlarm.h"
#include "AlarmHelper.h"
#include "MinorAlarmGenerator.h"
#include "Report.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NonUserAlarm::NonUserAlarm(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NonUserAlarm::getName() {
    return "NonUserAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NonUserAlarm::getDescription() {
    return "The <i>AlarmId</i> parameter indicates a non-User Alarm.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NonUserAlarm::getPrecondition() {
    return "Requires a non-User Alarm in the DAT or the ability to\n"
           "generate a non-user alarm by causing an UpMinor threshold\n"
           "event on a sensor.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NonUserAlarm::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 * 
 * Find a system alarm and try to delete it.
 *****************************************************************************/

HpiTestStatus NonUserAlarm::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    bool found;
    SaHpiAlarmT systemAlarm;

    status.add(TRACE, AlarmHelper::findSystemAlarm(sessionId,
                                                   &systemAlarm, &found));
    if (status.isOkay()) {
        if (found) {
            status.add(TRACE, testDelete(sessionId, systemAlarm));
        } else {
            MinorAlarmGenerator alarmGenerator(sessionId);
            status.add(TRACE, alarmGenerator.generateAlarm());
            if (status.isOkay()) {
                pauseForEventProcessing();  // wait for alarm to get into the DAT

                status.add(TRACE, AlarmHelper::findSystemAlarm(sessionId,
                                                               &systemAlarm,
                                                               &found));
                if (status.isOkay()) {
                    if (!found) {
                        status.assertNotSupport();
                    } else {
                        status.add(TRACE, testDelete(sessionId, systemAlarm));
                    }
                }
                alarmGenerator.clearAlarm();
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Test deleting a system alarm.
 *****************************************************************************/

HpiTestStatus NonUserAlarm::testDelete(SaHpiSessionIdT sessionId,
                                       SaHpiAlarmT &systemAlarm) {
    HpiTestStatus status;

    SaErrorT error = saHpiAlarmDelete(sessionId,
                                      systemAlarm.AlarmId, 
                                      systemAlarm.Severity);
    if (error == SA_ERR_HPI_READ_ONLY) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, ALARM_DELETE,
                             SA_ERR_HPI_READ_ONLY, error);
    }

    return status;
}

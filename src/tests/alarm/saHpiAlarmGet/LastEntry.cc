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

#include "LastEntry.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

LastEntry::LastEntry(char *line) : NonEmptyAlarmTestCase(line) {
    errorList.add(SA_OK);
    errorList.add(SA_ERR_HPI_NOT_PRESENT);
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *LastEntry::getName() {
    return "LastEntry";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *LastEntry::getDescription() {
    return "Use SAHPI_LAST_ENTRY as the <i>AlarmId</i> to obtain the last\n"
           "alarm in the DAT.  Since the specification is ambiguous, this\n"
           "may or may not work.";
}

/*****************************************************************************
 * Return the expected error codes.
 *****************************************************************************/

ErrorList *LastEntry::getExpectedReturnList() {
    return &errorList;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus LastEntry::runAlarmTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;

    status.assertPass();
    SaErrorT error = saHpiAlarmGet(sessionId, SAHPI_LAST_ENTRY, &alarm);
    if (error == SA_OK) {
        if (alarm.AlarmId == SAHPI_LAST_ENTRY) {
            status.assertFailure(TRACE, ALARM_GET, SA_OK, error);
        }
    } else if (error != SA_ERR_HPI_NOT_PRESENT) {
        status.assertFailure(TRACE, ALARM_GET, SA_OK,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

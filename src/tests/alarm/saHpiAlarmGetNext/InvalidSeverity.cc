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

#include "InvalidSeverity.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSeverity::InvalidSeverity(char *line) : NonEmptyAlarmTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getName() {
    return "InvalidSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getDescription() {
    return "The <i>Severity</i> parameter is not one of the valid enumerated\n"
           "values for this type.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidSeverity::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSeverity::runAlarmTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT alarm;
    int invalidSeverity[] = { (int) SAHPI_OK + 1, 
                              (int) SAHPI_DEBUG - 1, 
                              (int) SAHPI_DEBUG + 1,
                              (int) SAHPI_ALL_SEVERITIES - 1,
                              (int) SAHPI_ALL_SEVERITIES ,
                              (int) SAHPI_ALL_SEVERITIES + 1 };

    for (int i = 0; i < 6; i++) {
        alarm.AlarmId = SAHPI_FIRST_ENTRY;
        SaErrorT error = saHpiAlarmGetNext(sessionId,
                                           (SaHpiSeverityT) invalidSeverity[i],
                                           SAHPI_FALSE, &alarm);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ALARM_GET_NEXT,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

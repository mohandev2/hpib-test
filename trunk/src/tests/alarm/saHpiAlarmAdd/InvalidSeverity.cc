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

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSeverity::InvalidSeverity(char *line) : HpiSessionTestCase(line) {
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
    return "The <i>Alarm->Severity</i> is not one of the following enumerated\n"
           "values: SAHPI_MINOR, SAHPI_MAJOR, or SAHPI_CRITICAL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidSeverity::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * Try five invalid severities.
 *****************************************************************************/

HpiTestStatus InvalidSeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    int invalidSeverity[] = { (int) SAHPI_INFORMATIONAL,
                              (int) SAHPI_OK,
                              (int) SAHPI_OK + 1,
                              (int) SAHPI_DEBUG,
                              (int) SAHPI_ALL_SEVERITIES };

    for (int i = 0; i < 5; i++) {
        AlarmHelper::fill(&newAlarm);
        newAlarm.Severity = (SaHpiSeverityT) invalidSeverity[i];
        SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ALARM_ADD,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Severity: %s", 
                                 HpiString::severity((SaHpiSeverityT) invalidSeverity[i]));
        }
    }

    return status;
}

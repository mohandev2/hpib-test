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

using namespace ns_saHpiAlarmAcknowledge;

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
    return "The <i>AlarmId</i> parameter is SAHPI_ENTRY_UNSPECIFIED and\n"
           "<i>Severity</i> is not one of the valid enumerated values for\n"
           "this type.";
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

HpiTestStatus InvalidSeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    int invalidSeverity[] = { (int) SAHPI_OK + 1,
                              (int) SAHPI_DEBUG - 1,
                              (int) SAHPI_DEBUG + 1,
                              (int) SAHPI_ALL_SEVERITIES - 1,
                              (int) SAHPI_ALL_SEVERITIES + 1 };

    for (int i = 0; i < 5; i++) {
        SaErrorT error = saHpiAlarmAcknowledge(sessionId,
                                               SAHPI_ENTRY_UNSPECIFIED, 
                                               (SaHpiSeverityT) invalidSeverity[i]);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ALARM_ACKNOWLEDGE,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Severity: %s",
                                 HpiString::severity((SaHpiSeverityT) invalidSeverity[i]));
        }
    }

    return status;
}

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

#include "NoSeverity.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoSeverity::NoSeverity(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoSeverity::getName() {
    return "NoSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoSeverity::getDescription() {
    return "If the <i>AlarmId</i> parameter is passed as SAHPI_ENTRY_UNSPECIFIED,\n"
           "and no User Alarms are present that meet the specified <i>Severity</i>,\n"
           "the function will have no effect.  However, this is not an error.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoSeverity::getPrecondition() {
    return "Requires a severity that isn't being used by any User Alarms in the DAT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoSeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoSeverity::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    bool found;
    SaHpiSeverityT severity;

    status.add(TRACE, AlarmHelper::findUnusedSeverity(sessionId, &severity, &found));
    if (status.isOkay()) {
        if (!found) {
            status.assertNotSupport();
        } else {
            SaErrorT error = saHpiAlarmDelete(sessionId,
                                              SAHPI_ENTRY_UNSPECIFIED, severity);
            if (error == SA_OK) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ALARM_DELETE, SA_OK, error);
            }
        }
    }

    return status;
}

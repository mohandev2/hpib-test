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
#include "EventHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSeverity::InvalidSeverity(char *line) : EventLogTestCase(DISABLE_LOG, line) {
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
    return "The Severity is not one of the valid enumerated values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getPrecondition() {
    return "Must be able to add entries to an Event Log.";
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

HpiTestStatus InvalidSeverity::runLogTest(SaHpiSessionIdT sessionId,
                                          SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    int invalidSeverity[] = {
        (int) SAHPI_OK + 1,
        (int) SAHPI_DEBUG - 1,
        (int) SAHPI_DEBUG + 1,
        (int) SAHPI_ALL_SEVERITIES - 1,
        (int) SAHPI_ALL_SEVERITIES,
        (int) SAHPI_ALL_SEVERITIES + 1 };

    EventHelper::fill(&event);
    for (int i = 0; i < 6; i++) {
        event.Severity = (SaHpiSeverityT) invalidSeverity[i];
        SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
        if (error == SA_ERR_HPI_OUT_OF_SPACE) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Invalid Severity is %s",
                                 HpiString::severity((SaHpiSeverityT) invalidSeverity[i]));
        }
    }

    return status;
}

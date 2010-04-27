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

using namespace ns_saHpiEventAdd;

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
    return "The event structure passed via the <i>EvtEntry</i> parameter\n"
           "has an invalid <i>Severity</i>.";
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
    SaHpiEventT event;
    int invalidSeverity[] = { (int) SAHPI_OK + 1,
                              (int) SAHPI_DEBUG - 1,
                              (int) SAHPI_DEBUG + 1,
                              (int) SAHPI_ALL_SEVERITIES - 1,
                              (int) SAHPI_ALL_SEVERITIES,
                              (int) SAHPI_ALL_SEVERITIES + 1 };

    EventHelper::fill(&event);
    for (int i = 0; i < 6; i++) {
        event.Severity = (SaHpiSeverityT) invalidSeverity[i];
        SaErrorT error = saHpiEventAdd(sessionId, &event);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_ADD,
                                 SA_ERR_HPI_INVALID_PARAMS, error, 
                                 "Severity: %s",
                                 HpiString::severity((SaHpiSeverityT) invalidSeverity[i]));
        }
    }

    return status;
}

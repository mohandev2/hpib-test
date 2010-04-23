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

#include "SetSeverity.h"

using namespace ns_saHpiResourceSeveritySet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetSeverity::SetSeverity(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetSeverity::getName() {
    return "SetSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetSeverity::getDescription() {
    return "Set the severity level applied to event issued if a resource\n"
           "unexpectedly becomes unavailable to the HPI.  Try all of the valid\n"
           "severities: DEBUG, OK, INFORMATIONAL, MINOR, MAJOR, and CRITICAL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetSeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetSeverity::runResourceTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiSeverityT severity[] = { SAHPI_DEBUG, SAHPI_OK,
                                  SAHPI_INFORMATIONAL, SAHPI_MINOR,
                                  SAHPI_MAJOR, SAHPI_CRITICAL };

    // test all valid severities
      
    for (int i = 0; i < 6; i++) {
        status.add(TRACE, setSeverity(sessionId, rptEntry->ResourceId, severity[i]));
    }
      
    // Restore the original severity

    SaErrorT error = saHpiResourceSeveritySet(sessionId,
                                              rptEntry->ResourceId,
                                              rptEntry->ResourceSeverity);
    status.checkError(TRACE, RESOURCE_SEVERITY_SET, error);

    return status;
}

/*****************************************************************************
 * Set the severity for a resource.  Verify that the change did occur
 * by reading the RPT entry.
 *****************************************************************************/

HpiTestStatus SetSeverity::setSeverity(SaHpiSessionIdT sessionId, 
                                SaHpiResourceIdT resourceId,
                                SaHpiSeverityT severity) {
    HpiTestStatus status;
    SaHpiRptEntryT rptEntry;

    SaErrorT error = saHpiResourceSeveritySet(sessionId, resourceId, severity);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_SEVERITY_SET, SA_OK, error, 
                          "Severity is %s.", HpiString::severity(severity));
    } else {
        error = saHpiRptEntryGetByResourceId(sessionId, resourceId, &rptEntry);
        if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
        } else if (rptEntry.ResourceSeverity != severity) {
            status.assertFailure(TRACE, "saHpiResourceSeveritySet() failed to "
                                 "actually set the severity.\n"
                                 "New Severity: %s; RptEntry Severity: %s.", 
                                 HpiString::severity(severity), 
                                 HpiString::severity(rptEntry.ResourceSeverity));
        } else {
            status.assertPass();
        }
    }

    return status;
}


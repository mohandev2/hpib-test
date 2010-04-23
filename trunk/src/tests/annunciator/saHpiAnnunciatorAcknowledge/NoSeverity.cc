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
#include "AnnouncementSet.h"

using namespace ns_saHpiAnnunciatorAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoSeverity::NoSeverity(char *line) : AnnunciatorTestCase(line) {
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
    return "If the <i>EntryId</i> parameter is as SAHPI_ENTRY_UNSPECIFIED,\n"
           "and no announcements are present that meet the requested\n"
           "<i>Severity</i>, this function will have no effect.  However,\n"
           "this is not an error.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoSeverity::getPrecondition() {
    return "Requires an Annunciator for which a severity can be found\n"
           "that isn't used by any of its announcements.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoSeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Find the severities that are not being used by any announcements.
 * Acknowledge using those severities.
 *****************************************************************************/

HpiTestStatus NoSeverity::runAnnunTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    int severityCount;
    SaHpiSeverityT severity[MAX_SEVERITIES];
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    status.add(TRACE, getUnusedSeverities(sessionId, resourceId, annunNum,
                                          SAHPI_FALSE, severity, &severityCount));
    if (status.isOkay()) {
        if (severityCount == 0) {
            status.assertNotSupport();   // can't find an unused severity
        } else {
            for (int i = 0; i < severityCount; i++) {
                SaErrorT error = saHpiAnnunciatorAcknowledge(sessionId,
                                                             resourceId,
                                                             annunNum,
                                                             SAHPI_ENTRY_UNSPECIFIED,
                                                             severity[i]);
                if (error == SA_OK) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, ANNUNCIATOR_ACKNOWLEDGE, SA_OK, error,
                                         "Severity = %s", HpiString::severity(severity[i]));
                }
            }
        }
    }

    return status;
}

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

using namespace ns_saHpiAnnunciatorDelete;

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
    return "If the <i>EntryId</i> parameter is passed as\n"
           "SAHPI_ENTRY_UNSPECIFIED, and no announcements are present that\n"
           "meet the specified <i>Severity</i>, this function will have no\n"
           "effect.  However, this is not an error.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoSeverity::getPrecondition() {
    return "Requires an Annunciator that is in USER or SHARED mode or can be\n"
           "placed in USER or SHARED mode. A severity must also be found that\n"
           "isn't being used by any of the announcements in that Annunciator.";
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
 * Find a severity that isn't used by any of the announcements and then
 * use that severity for the deletion test.  If we can't find a severity,
 * then delete the INFORMATIONAL announcements and then use that severity.
 *****************************************************************************/

HpiTestStatus NoSeverity::runAnnunTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    bool found;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiSeverityT severity;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {

        status.add(TRACE, getUnusedSeverity(sessionId, resourceId, annunNum,
                                            SAHPI_TRUE, &severity, &found));
        if (status.isOkay()) {
            if (found) {
                SaErrorT error = saHpiAnnunciatorDelete(sessionId, resourceId,
                                                        annunNum,
                                                        SAHPI_ENTRY_UNSPECIFIED,
                                                        severity);
                if (error == SA_OK) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, ANNUNCIATOR_DELETE, SA_OK, error,
                                         "Severity is %s.", HpiString::severity(severity));
                }
            } else {
                SaErrorT error = saHpiAnnunciatorDelete(sessionId, resourceId,
                                                        annunNum,
                                                        SAHPI_ENTRY_UNSPECIFIED,
                                                        SAHPI_INFORMATIONAL);
                if (error != SA_OK) {
                    status.assertError(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
                } else {
                    error = saHpiAnnunciatorDelete(sessionId, resourceId, annunNum,
                                                   SAHPI_ENTRY_UNSPECIFIED,
                                                   SAHPI_INFORMATIONAL);
                    if (error == SA_OK) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
                    }
                }
            }
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

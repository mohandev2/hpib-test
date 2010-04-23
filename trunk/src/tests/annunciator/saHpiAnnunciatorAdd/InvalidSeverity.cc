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

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSeverity::InvalidSeverity(char *line) : AnnunciatorTestCase(line) {
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
    return "The <i>Announcement->Severity</i> passed is not one of the\n"
           "valid enumerated values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
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
 * Test five invalid severities.
 *****************************************************************************/

HpiTestStatus InvalidSeverity::runAnnunTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;
    int invalidSeverity[] = { (int) SAHPI_OK + 1,
                              (int) SAHPI_DEBUG - 1,
                              (int) SAHPI_DEBUG + 1,
                              (int) SAHPI_ALL_SEVERITIES - 1,
                              (int) SAHPI_ALL_SEVERITIES,
                              (int) SAHPI_ALL_SEVERITIES + 1 };

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        for (int i = 0; i < 6; i++) {
            AnnunciatorHelper::fill(&announcement, (SaHpiSeverityT) invalidSeverity[i]);
            SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
                                                 annunNum, &announcement);
            if (error == SA_ERR_HPI_INVALID_PARAMS) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ANNUNCIATOR_ADD,
                                     SA_ERR_HPI_INVALID_PARAMS, error,
                                     "Invalid Severity is %s.", 
                                     HpiString::severity((SaHpiSeverityT) invalidSeverity[i]));
            } 
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

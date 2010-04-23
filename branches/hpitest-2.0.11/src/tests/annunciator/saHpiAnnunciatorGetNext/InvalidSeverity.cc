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

using namespace ns_saHpiAnnunciatorGetNext;

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
    return "The <i>Severity</i> is not one of the valid enumerated values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidSeverity::getPrecondition() {
    return "Requires an Annunciator.";
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
 * Test a number of invalid severities.
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
                              (int) SAHPI_ALL_SEVERITIES + 1 };

    for (int i = 0; i < 5; i++) {
        announcement.EntryId = SAHPI_FIRST_ENTRY;
        SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                                 (SaHpiSeverityT) invalidSeverity[i],
                                                 SAHPI_FALSE, &announcement);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_GET_NEXT,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

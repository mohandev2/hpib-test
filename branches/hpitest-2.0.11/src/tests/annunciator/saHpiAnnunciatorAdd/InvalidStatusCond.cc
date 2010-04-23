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

#include "InvalidStatusCond.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidStatusCond::InvalidStatusCond(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidStatusCond::getName() {
    return "InvalidStatusCond";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidStatusCond::getDescription() {
    return "The <i>Announcement->StatusCond</i> structure passed is invalid.\n"
           "The type is not SAHPI_STATUS_COND_TYPE_USER.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidStatusCond::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidStatusCond::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * Test four invalid status condition types.
 *****************************************************************************/

HpiTestStatus InvalidStatusCond::runAnnunTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;
    int invalidCondType[] = { (int) SAHPI_STATUS_COND_TYPE_USER + 1 };

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        for (int i = 0; i < 1; i++) {
            AnnunciatorHelper::fill(&announcement);
            announcement.StatusCond.Type = (SaHpiStatusCondTypeT) invalidCondType[i];
            SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
                                                 annunNum, &announcement);
            if (error == SA_ERR_HPI_INVALID_PARAMS) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ANNUNCIATOR_ADD,
                                     SA_ERR_HPI_INVALID_PARAMS, error,
                                     "StatusCond.Type = %s.",
                                     HpiString::statusCondType(
                                         (SaHpiStatusCondTypeT) invalidCondType[i]));
            } 
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

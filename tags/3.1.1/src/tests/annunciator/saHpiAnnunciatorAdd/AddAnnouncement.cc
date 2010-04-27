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

#include "AddAnnouncement.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddAnnouncement::AddAnnouncement(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddAnnouncement::getName() {
    return "AddAnnouncement";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddAnnouncement::getDescription() {
    return "Add announcements with each of the valid severities.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AddAnnouncement::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddAnnouncement::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add annoucements with different valid severities.
 *****************************************************************************/

HpiTestStatus AddAnnouncement::runAnnunTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    int severityCount;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        SaHpiSeverityT *severity = getValidSeverities(&severityCount);
        for (int i = 0; i < severityCount; i++) {
            AnnunciatorHelper::fill(&announcement, severity[i]);
            SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
                                                 annunNum, &announcement);
            if (error == SA_ERR_HPI_OUT_OF_SPACE) {
                status.assertNotSupport();
            } else if (error != SA_OK) {
                status.assertFailure(TRACE, ANNUNCIATOR_ADD, SA_OK, error);
            } else {
                status.assertPass();
                status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                     annunNum, &announcement));
            }
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

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

#include "UnacknowledgedTraversal.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

UnacknowledgedTraversal::UnacknowledgedTraversal(char *line)
: AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *UnacknowledgedTraversal::getName() {
    return "UnacknowledgedTraversal";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *UnacknowledgedTraversal::getDescription() {
    return "Retrieve all of the unacknowledged announcements in the Annunciator.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *UnacknowledgedTraversal::getPrecondition() {
    return "Requires an Annunciator that has one or more unacknowledged\n"
           "announcements or an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT UnacknowledgedTraversal::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * This test tries to be as comprehensive as possible.  In the first case,
 * it is not possible to add new announcements to the annunciator.  In this
 * situation, we traverse the table using getAnnouncementCount().  If the
 * number of unacknowledge announcement in the table is greater than zero,
 * then we can feel safe to say that the traversal is okay.  If it is zero,
 * we can't say that the traversal works and thus the status will be not 
 * supported.
 *
 * In the second case, we should be able to add a full set of announcements
 * that includes all combinations of severities and acknowledgements.  We
 * then will do the traversal and expect to see at least one unacknowledged
 * announcement per severity.
 *****************************************************************************/

HpiTestStatus UnacknowledgedTraversal::runAnnunTest(SaHpiSessionIdT sessionId,
                                                    SaHpiRptEntryT *rptEntry,
                                                    SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    int count;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    HpiTestStatus writeStatus = setWriteMode(sessionId, resourceId, annunRec);
    if (writeStatus.hasFault()) {
        status.add(TRACE, writeStatus);
    } else if (writeStatus.isNotSupported()) {
        status.add(TRACE, getAnnouncementCount(sessionId, resourceId, annunNum,
                                               SAHPI_ALL_SEVERITIES,
                                               SAHPI_TRUE, &count));
        if (status.isOkay()) {
            if (count > 0) {
                status.assertPass();
            } else {
                status.assertNotSupport();
            }
        }
    } else {
        status.add(TRACE, addTestAnnouncements(sessionId, resourceId, annunNum));
        if (status.isOkay()) {
            status.add(TRACE, getAnnouncementCount(sessionId, resourceId,
                                                   annunNum, SAHPI_ALL_SEVERITIES,
                                                   SAHPI_TRUE, &count));
            if (status.isOkay()) {
                if (count >= MAX_SEVERITIES) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, 
                        "Did not get all of the unacknowledged announcements.");
                }
            }
        }

        status.add(TRACE, deleteTestAnnouncements(sessionId, resourceId, annunNum));
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}


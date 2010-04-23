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

#include "SeverityTraversal.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SeverityTraversal::SeverityTraversal(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SeverityTraversal::getName() {
    return "SeverityTraversal";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SeverityTraversal::getDescription() {
    return "Traverse the Annunciator for announcements of a specific\n"
           "severity.  All of the valid severities will be tested.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SeverityTraversal::getPrecondition() {
    return "Requires an Annunciator that has one or more announcements or\n"
           "an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SeverityTraversal::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * This test tries to be as comprehensive as possible.  In the first case,
 * it is not possible to add new announcements to the annunciator.  In this
 * situation, we traverse the table using getAnnouncementCount().  If the
 * number of entries in the table is greater than zero for any of the 
 * specific severities, then we can feel safe to say that the traversal is 
 * okay.  If it is zero for all of the severities, we can't say that the 
 * traversal works and thus the status will be not supported.
 *
 * In the second case, we should be able to add a full set of announcements
 * that includes all combinations of severities and acknowledgements.  We
 * then will do the traversal and expect to see at least two announcements
 * for each severity in the annunciator.
 *****************************************************************************/

HpiTestStatus SeverityTraversal::runAnnunTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    int count, severityCount;

    HpiTestStatus writeStatus = setWriteMode(sessionId, resourceId, annunRec);
    if (writeStatus.hasFault()) {
        status.add(TRACE, writeStatus);
    } else if (writeStatus.isNotSupported()) {
        SaHpiSeverityT *severity = getValidSeverities(&severityCount);
        for (int i = 0; i < severityCount && !status.hasFault(); i++) {
            status.add(TRACE, getAnnouncementCount(sessionId, resourceId,
                                                   annunNum, severity[i],
                                                   SAHPI_FALSE, &count));
            if (status.isOkay()) {
                if (count > 0) {
                    status.assertPass();
                } else {
                    status.assertNotSupport();
                }
            }
        }
    } else {
        status.add(TRACE, addTestAnnouncements(sessionId, resourceId, annunNum));
        if (status.isOkay()) {
            SaHpiSeverityT *severity = getValidSeverities(&severityCount);
            for (int i = 0; i < severityCount && !status.hasFault(); i++) {
                status.add(TRACE, getAnnouncementCount(sessionId, resourceId, annunNum, 
                                                       severity[i], SAHPI_FALSE, &count));
                if (status.isOkay()) {
                    if (count >= 2) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, 
                                    "Did not read all announcements of severity %s.", 
                                    HpiString::severity(severity[i]));
                    }
                }
            }
        }

        status.add(TRACE, deleteTestAnnouncements(sessionId, resourceId, annunNum));
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    } 

    return status;
}

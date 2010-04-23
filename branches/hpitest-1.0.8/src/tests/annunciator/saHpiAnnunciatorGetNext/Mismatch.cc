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

#include "Mismatch.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Mismatch::Mismatch(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Mismatch::getName() {
    return "Mismatch";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Mismatch::getDescription() {
    return "The passed <i>Announcement->EntryId</i> matches an announcement\n"
           "in the current set, but the passed <i>Announcement->Timestamp</i>\n"
           "does not match the timestamp of the announcement.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Mismatch::getPrecondition() {
    return "Requires an Annunciator with two or more announcements or an\n"
           "Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Mismatch::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *
 * To run the mistmatch test, we must have at least two announcements in
 * the annunciator table.
 *****************************************************************************/

HpiTestStatus Mismatch::runAnnunTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    int count;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    status.add(TRACE, getAnnouncementCount(sessionId, resourceId, annunNum,
                                           SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                           &count));
    if (status.isOkay()) {
        if (count >= 2) {
            status.add(TRACE, testMismatch(sessionId, resourceId, annunNum));
        } else {
            status.add(TRACE, prepAndTestMismatch(sessionId, resourceId, annunRec));
        }
    }

    return status;
}

/*****************************************************************************
 * If possible, add two announcements before running the mismatch test.
 *****************************************************************************/

HpiTestStatus Mismatch::prepAndTestMismatch(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId,
                                            SaHpiAnnunciatorRecT * annunRec) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement1;
    SaHpiAnnouncementT announcement2;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                              annunNum, &announcement1));
        if (status.isOkay()) {
            status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                                  annunNum, &announcement2));
            if (status.isOkay()) {
                status.add(TRACE, testMismatch(sessionId, resourceId, annunNum));

                status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                     annunNum, &announcement2));
            }
            status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                 annunNum, &announcement1));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

/*****************************************************************************
 * Get the first announcement and then use that announcement to get the
 * next announcement, but first create a mismatch by changing the timestamp
 * of the first announcement before retrieving the next announcement.
 *****************************************************************************/

HpiTestStatus Mismatch::testMismatch(SaHpiSessionIdT sessionId,
                                     SaHpiResourceIdT resourceId,
                                     SaHpiAnnunciatorNumT annunNum) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;

    // Get the first announcement.

    announcement.EntryId = SAHPI_FIRST_ENTRY;
    SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                             SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                             &announcement);
    if (error != SA_OK) {
        status.assertError(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, error);
    } else {

        // Increment the Timestamp to create the mismatch and
        // try retrieving another announcement.  This should
        // result in an error condition.

        announcement.Timestamp++;
        error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                        SAHPI_ALL_SEVERITIES, SAHPI_FALSE, 
                                        &announcement);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_GET_NEXT,
                                 SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}


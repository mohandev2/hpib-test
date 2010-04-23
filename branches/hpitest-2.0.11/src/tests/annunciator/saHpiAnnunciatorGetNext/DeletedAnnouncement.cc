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

#include "DeletedAnnouncement.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DeletedAnnouncement::DeletedAnnouncement(char *line)
: AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DeletedAnnouncement::getName() {
    return "DeletedAnnouncement";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DeletedAnnouncement::getDescription() {
    return "The function saHpiAnnunciatorGetNext() will operate even if\n"
           "the announcement associated with the <i>EntryId</i> and\n"
           "<i>Timestamp</i> passed by the HPI User has been deleted.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DeletedAnnouncement::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DeletedAnnouncement::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add two announcements and then, in the below method, delete the first
 * announcement and invoke saHpiAnnunciatorGetNext() with that deleted
 * announcement.  The next announcement should be returned.  Note that the
 * second announcement must be added in order to avoid NOT_PRESENT being
 * returned for reaching the end of the table.
 *****************************************************************************/

HpiTestStatus DeletedAnnouncement::runAnnunTest(SaHpiSessionIdT sessionId,
                                                SaHpiRptEntryT *rptEntry,
                                                SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement1;
    SaHpiAnnouncementT announcement2;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                              annunNum, &announcement1));
        if (status.isOkay()) {
            status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                                  annunNum, &announcement2));
            if (status.isOkay()) {
                status.add(TRACE, testDeleteAndGet(sessionId, resourceId,
                                                   annunNum, &announcement1));

                status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                     annunNum, &announcement2));
            } else {
                status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                     annunNum, &announcement1));
            }
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

/*****************************************************************************
 * Delete the given announcement and then use that announcement to get the
 * next announcement.
 *****************************************************************************/

HpiTestStatus DeletedAnnouncement::testDeleteAndGet(SaHpiSessionIdT sessionId,
                                                    SaHpiResourceIdT resourceId, 
                                                    SaHpiAnnunciatorNumT annunNum,
                                                    SaHpiAnnouncementT *announcement) {
    HpiTestStatus status;

    SaErrorT error = saHpiAnnunciatorDelete(sessionId, resourceId, annunNum, 
                                            announcement->EntryId, SAHPI_ALL_SEVERITIES);
    if (error != SA_OK) {
        status.assertError(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
    } else {
        error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                        SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                        announcement);
        if (error == SA_OK) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, error);
        }
    }

    return status;
}

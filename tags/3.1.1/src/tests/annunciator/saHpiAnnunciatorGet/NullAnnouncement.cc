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

#include "NullAnnouncement.h"

using namespace ns_saHpiAnnunciatorGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullAnnouncement::NullAnnouncement(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullAnnouncement::getName() {
    return "NullAnnouncement";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullAnnouncement::getDescription() {
    return "The <i>Announcement</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullAnnouncement::getPrecondition() {
    return "Requires an Annunciator with an announcement or an Annunciator\n"
           "for which an announcement can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullAnnouncement::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullAnnouncement::runAnnunTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    // Try using an announcement that is in the table in order to
    // use a valid entryId.  If that fails, the below method will
    // try to add an announcement in order to do this test.

    announcement.EntryId = SAHPI_FIRST_ENTRY;
    SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                             SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                             &announcement);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.add(TRACE, addAndTest(sessionId, resourceId, annunRec));
    } else if (error != SA_OK) {
        status.assertError(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, error);
    } else {
        error = saHpiAnnunciatorGet(sessionId, resourceId, annunNum,
                                    announcement.EntryId, NULL);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_GET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Try adding an announcement to the table and then use it's entryId for
 * the test case.
 *****************************************************************************/

HpiTestStatus NullAnnouncement::addAndTest(SaHpiSessionIdT sessionId,
                                           SaHpiResourceIdT resourceId,
                                           SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                              annunNum, &announcement));
        if (status.isOkay()) {
            SaErrorT error = saHpiAnnunciatorGet(sessionId, resourceId, annunNum,
                                                 announcement.EntryId, NULL);
            if (error == SA_ERR_HPI_INVALID_PARAMS) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ANNUNCIATOR_GET,
                                     SA_ERR_HPI_INVALID_PARAMS, error);
            }

            status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                 annunNum, &announcement));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

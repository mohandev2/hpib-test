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

#include "AcknowledgeAnnouncement.h"

using namespace ns_saHpiAnnunciatorAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeAnnouncement::AcknowledgeAnnouncement(char *line)
: AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeAnnouncement::getName() {
    return "AcknowledgeAnnouncement";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeAnnouncement::getDescription() {
    return "Acknowledge a single announcement and verify that the\n"
           "acknowledgment was successful.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AcknowledgeAnnouncement::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeAnnouncement::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an announcement, acknowledge it, and then verify that it worked
 * correctly.
 *****************************************************************************/

HpiTestStatus AcknowledgeAnnouncement::runAnnunTest(SaHpiSessionIdT sessionId,
                                                    SaHpiRptEntryT *rptEntry,
                                                    SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;
    SaHpiAnnouncementT ackAnnouncement;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                              annunNum, &announcement));
        if (status.isOkay()) {
            SaErrorT error = saHpiAnnunciatorAcknowledge(sessionId, resourceId,
                                                         annunNum, 
                                                         announcement.EntryId,
                                                         SAHPI_ALL_SEVERITIES);
            if (error != SA_OK) {
                status.assertFailure(TRACE, ANNUNCIATOR_ACKNOWLEDGE, SA_OK, error);
            } else {
                error = saHpiAnnunciatorGet(sessionId, resourceId, annunNum,
                                            announcement.EntryId, &ackAnnouncement);
                if (error != SA_OK) {
                    status.assertError(TRACE, ANNUNCIATOR_GET, SA_OK, error);
                } else if (ackAnnouncement.Acknowledged) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, "Failed to acknowledge announcement; "
                                                "Acknowledged field is False.");
                }
            }

            status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                 annunNum, &announcement));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

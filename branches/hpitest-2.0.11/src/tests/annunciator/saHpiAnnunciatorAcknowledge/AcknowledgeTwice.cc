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

#include "AcknowledgeTwice.h"

using namespace ns_saHpiAnnunciatorAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeTwice::AcknowledgeTwice(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeTwice::getName() {
    return "AcknowledgeTwice";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeTwice::getDescription() {
    return "If an announcement has been previously acknowledged, acknowledging\n"
           "it again has no effect, However, this is not an error.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AcknowledgeTwice::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeTwice::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an announcement and acknowledge it twice.  This is legal.
 *****************************************************************************/

HpiTestStatus AcknowledgeTwice::runAnnunTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                              annunNum, &announcement));
        if (status.isOkay()) {
            SaErrorT error = saHpiAnnunciatorAcknowledge(sessionId,
                                                         resourceId, annunNum, 
                                                         announcement.EntryId,
                                                         SAHPI_ALL_SEVERITIES);
            if (error != SA_OK) {
                status.assertError(TRACE, ANNUNCIATOR_ACKNOWLEDGE, SA_OK, error);
            } else {
                error = saHpiAnnunciatorAcknowledge(sessionId,
                                                    resourceId, annunNum,
                                                    announcement.EntryId,
                                                    SAHPI_ALL_SEVERITIES);
                if (error == SA_OK) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, ANNUNCIATOR_ACKNOWLEDGE, SA_OK, error);
                }
            }

            status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                 annunNum, &announcement));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

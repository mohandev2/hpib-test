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

#include "AcknowledgeAll.h"
#include "AnnouncementSet.h"

using namespace ns_saHpiAnnunciatorAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AcknowledgeAll::AcknowledgeAll(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AcknowledgeAll::getName() {
    return "AcknowledgeAll";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AcknowledgeAll::getDescription() {
    return "To acknowledge all announcements contained within the current\n"
           "set, set the <i>Severity</i> parameter to SAHPI_ALL_SEVERITIES,\n"
           "and set the <i>EntryId</i> parameter to SAHPI_ENTRY_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AcknowledgeAll::getPrecondition() {
    return "Requires an Annunciator for which an announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AcknowledgeAll::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add a set of annunciators with various severities just because we want
 * a bunch of announcements in the annunciator and we will, of course, have
 * something to acknowledge.  After acknowledging everything, read the
 * announcements back and verify that they are all acknowledged.
 *****************************************************************************/

HpiTestStatus AcknowledgeAll::runAnnunTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT ackAnnouncement;
    AnnouncementSet announcementSet;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addSeverityAnnouncements(sessionId, resourceId,
                                                   annunNum, &announcementSet));
        if (status.isOkay()) {
            SaErrorT error = saHpiAnnunciatorAcknowledge(sessionId,
                                                         resourceId, annunNum, 
                                                         SAHPI_ENTRY_UNSPECIFIED,
                                                         SAHPI_ALL_SEVERITIES);
            if (error != SA_OK) {
                status.assertFailure(TRACE, ANNUNCIATOR_ACKNOWLEDGE, SA_OK, error);
            } else {
                ackAnnouncement.EntryId = SAHPI_FIRST_ENTRY;
                while (status.isOkay()) {
                    error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                                    SAHPI_ALL_SEVERITIES,
                                                    SAHPI_FALSE, &ackAnnouncement);
                    if (error == SA_ERR_HPI_NOT_PRESENT) {
                        break;
                    } else if (error != SA_OK) {
                        status.assertError(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, error);
                    } else if (ackAnnouncement.Acknowledged) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, 
                                             "Failed to acknowledge all announcements; "
                                             "Acknowledged field is False.");
                    }
                }
            }

            status.add(TRACE, deleteAnnouncements(sessionId, resourceId,
                                                  annunNum, &announcementSet));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

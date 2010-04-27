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

#include "NoSeverityAnnouncements.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoSeverityAnnouncements::NoSeverityAnnouncements(char *line)
: AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoSeverityAnnouncements::getName() {
    return "NoSeverityAnnouncements";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoSeverityAnnouncements::getDescription() {
    return "There are no announcements that meet the specified <i>Severity</i>\n"
           "in the current set for a passed <i>Announcement->EntryId</i> field\n"
           "that is set to SAHPI_FIRST_ENTRY.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoSeverityAnnouncements::getPrecondition() {
    return "Requires an Annunciator with a severity that isn't used by\n"
           "any of it's announcements or an Annunciator whose announcements\n"
           "can be deleted.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoSeverityAnnouncements::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *
 * Try to find a severity that isn't being used by any of the announcements
 * in the annunciator table.  If none is found, we will do a prep that first
 * deletes all of the INFORMATIONAL announcements before running the test.
 *****************************************************************************/

HpiTestStatus NoSeverityAnnouncements::runAnnunTest(SaHpiSessionIdT sessionId,
                                                    SaHpiRptEntryT *rptEntry,
                                                    SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiSeverityT severity;
    bool found;

    status.add(TRACE, getUnusedSeverity(sessionId, resourceId, annunNum,
                                        SAHPI_FALSE, &severity, &found));
    if (status.isOkay()) {
        if (found) {
            status.add(TRACE, runNotPresentTest(sessionId, resourceId, annunNum, severity));
        } else {
            status.add(TRACE, prepAndRunTest(sessionId, resourceId, annunRec));
        }
    }

    return status;
}

/*****************************************************************************
 * If possible, delete all INFORMATIONAL announcements before running the
 * test.
 *****************************************************************************/

HpiTestStatus NoSeverityAnnouncements::prepAndRunTest(SaHpiSessionIdT sessionId, 
                                                      SaHpiResourceIdT resourceId, 
                                                      SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiSeverityT severity = SAHPI_INFORMATIONAL;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {

        SaErrorT error = saHpiAnnunciatorDelete(sessionId, resourceId,
                                                annunRec->AnnunciatorNum,
                                                SAHPI_ENTRY_UNSPECIFIED, severity);
        if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
        } else {
            status.add(TRACE, runNotPresentTest(sessionId, resourceId,
                                      annunRec->AnnunciatorNum, severity));
        }

        status.add(TRACE, restoreMode(sessionId, resourceId, annunRec->AnnunciatorNum));
    }

    return status;
}

/*****************************************************************************
 * Try to get the first announcement that has the given severity.  There
 * shouldn't be any.
 *****************************************************************************/

HpiTestStatus NoSeverityAnnouncements::runNotPresentTest(SaHpiSessionIdT sessionId,
                                                         SaHpiResourceIdT resourceId, 
                                                         SaHpiAnnunciatorNumT annunNum,
                                                         SaHpiSeverityT severity) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;

    announcement.EntryId = SAHPI_FIRST_ENTRY;
    SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                             severity, SAHPI_FALSE, &announcement);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, ANNUNCIATOR_GET_NEXT,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

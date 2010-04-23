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

#include "SeverityDelete.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SeverityDelete::SeverityDelete(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SeverityDelete::getName() {
    return "SeverityDelete";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SeverityDelete::getDescription() {
    return "Delete all announcements of a specific severity and verify\n"
           "that those announcements have been deleted from the Annunciator.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SeverityDelete::getPrecondition() {
    return "Requires an Annunciator that is in USER or SHARED mode or can\n"
           "be placed in USER or SHARED mode.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SeverityDelete::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Get a severity that isn't being used by any announcements (if could not
 * find one, INFORMATIONAL will be used) and then add an announcement of
 * that severity.  Do the deletion test and verify that the announcement
 * is gone.
 *****************************************************************************/

HpiTestStatus SeverityDelete::runAnnunTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    bool found;
    SaHpiSeverityT severity;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, getUnusedSeverity(sessionId, resourceId, annunNum,
                                            SAHPI_TRUE, &severity, &found));
        if (status.isOkay()) {
            status.add(TRACE, addAnnouncement(sessionId, resourceId,
                                              annunNum, severity, &announcement));
            if (status.isOkay()) {
                SaErrorT error = saHpiAnnunciatorDelete(sessionId,
                                                        resourceId, annunNum,
                                                        SAHPI_ENTRY_UNSPECIFIED,
                                                        severity);
                if (error != SA_OK) {
                    status.assertFailure(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
                } else {
                    status.add(TRACE, containsAnnouncement(sessionId,
                                                           resourceId,
                                                           annunNum,
                                                           announcement.EntryId,
                                                           &found));
                    if (status.isOkay()) {
                        if (!found) {
                            status.assertPass();
                        } else {
                            status.assertFailure(TRACE, 
                                "Failed to delete announcement 0x%X with a severity of %s.", 
                                announcement.EntryId, HpiString::severity(severity));
                        }
                    }
                }
            }
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

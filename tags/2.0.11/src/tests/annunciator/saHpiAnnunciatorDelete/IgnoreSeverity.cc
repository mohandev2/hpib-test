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

#include "IgnoreSeverity.h"

using namespace ns_saHpiAnnunciatorDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IgnoreSeverity::IgnoreSeverity(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getName() {
    return "IgnoreSeverity";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getDescription() {
    return "The <i>Severity</i> parameter is ignored when the\n"
           "<i>EntryId</i> parameter is set to a value other than\n"
           "SAHPI_ENTRY_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *IgnoreSeverity::getPrecondition() {
    return "Requires an Annunciator for which an announcement can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IgnoreSeverity::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an INFO announcement and then delete it using a severity of MINOR.
 * Verify that the delete worked correctly.
 *****************************************************************************/

HpiTestStatus IgnoreSeverity::runAnnunTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    bool found;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addInfoAnnouncement(sessionId, resourceId,
                                              annunNum, &announcement));
        if (status.isOkay()) {
            SaErrorT error = saHpiAnnunciatorDelete(sessionId,
                                                    resourceId, annunNum,
                                                    announcement.EntryId,
                                                    SAHPI_MINOR);
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
                             "Failed to delete announcement 0x%X.", announcement.EntryId);
                    }
                }
            }
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

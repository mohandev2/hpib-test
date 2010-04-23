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

#include "DeleteAll.h"

using namespace ns_saHpiAnnunciatorDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DeleteAll::DeleteAll(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DeleteAll::getName() {
    return "DeleteAll";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DeleteAll::getDescription() {
    return "To delete all announcements contained within the Annunciator,\n"
           "set the <i>EntryId</i> parameter to SAHPI_ENTRY_UNSPECIFIED and\n"
           "the <i>Severity</i> parameter to SAHPI_ALL_SEVERITIES.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DeleteAll::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DeleteAll::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add a set of announcements with different severities so that we have
 * something to delete and then go ahead and delete them and verify that
 * the Annunicator is empty afterwards.
 *****************************************************************************/

HpiTestStatus DeleteAll::runAnnunTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    bool empty;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    AnnouncementSet announcementSet;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        status.add(TRACE, addSeverityAnnouncements(sessionId, resourceId,
                                                   annunNum, &announcementSet));
        if (status.isOkay()) {
            SaErrorT error = saHpiAnnunciatorDelete(sessionId,
                                                    resourceId, annunNum,
                                                    SAHPI_ENTRY_UNSPECIFIED,
                                                    SAHPI_ALL_SEVERITIES);
            if (error != SA_OK) {
                status.assertFailure(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
            } else {
                status.add(TRACE, isEmpty(sessionId, resourceId, annunNum, &empty));
                if (status.isOkay()) {
                    if (empty) {
                        status.assertPass();
                    } else {
                        status.assertFailure(TRACE, 
                                    "Failed to delete all of the announcements.");
                    }
                }
            }
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

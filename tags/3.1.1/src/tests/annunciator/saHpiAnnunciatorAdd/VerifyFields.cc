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

#include "VerifyFields.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

VerifyFields::VerifyFields(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *VerifyFields::getName() {
    return "VerifyFields";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *VerifyFields::getDescription() {
    return "Upon successful addition of an announcement, the <i>EntryId</i>,\n"
           "<i>Timestamp</i>, and <i>AddedByUser</i> fields within the\n"
           "<i>Announcement</i> parameter will be set to new values associated\n"
           "with the added announcement.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *VerifyFields::getPrecondition() {
    return "Requires an Annunciator for which an announcement can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT VerifyFields::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Add an announcement and then verify that the fields were set correctly.
 *****************************************************************************/

HpiTestStatus VerifyFields::runAnnunTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));
    if (status.isOkay()) {
        AnnunciatorHelper::fill(&announcement);

        announcement.EntryId = SAHPI_FIRST_ENTRY;
        announcement.Timestamp = -1;
        announcement.AddedByUser = SAHPI_FALSE;

        SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
                                             annunNum, &announcement);
        if (error == SA_ERR_HPI_OUT_OF_SPACE) {
            status.assertNotSupport();
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, ANNUNCIATOR_ADD, SA_OK, error);
        } else { 
            status.assertPass();

            if (announcement.EntryId == SAHPI_FIRST_ENTRY) {
                status.assertFailure(TRACE, 
                        "Failed to set the announcement's EntryId field.");
            }

            if (announcement.Timestamp == -1) {
                status.assertFailure(TRACE, 
                        "Failed to set the announcement's Timestamp field.");
            }

            if (!announcement.AddedByUser) {
                status.assertFailure(TRACE, 
                        "Failed to set the announcement's AddedByUser field to TRUE.");
            }

            status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                 annunNum, &announcement));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

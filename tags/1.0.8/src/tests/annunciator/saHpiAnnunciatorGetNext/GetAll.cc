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

#include "GetAll.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAll::GetAll(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAll::getName() {
    return "GetAll";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAll::getDescription() {
    return "Retrieve all of the announcements in the Annunciator.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetAll::getPrecondition() {
    return "The Annunciator must contain one or more entries or be\n"
           "capable of adding announcements.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAll::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * This test tries to be as comprehensive as possible.  In the first case,
 * it is not possible to add new announcements to the annunciator.  In this
 * situation, we traverse the table using getAnnouncementCount().  If the
 * number of entries in the table is greater than zero, then we feel safe to
 * say that the traversal is okay.  If it is zero, we can't say that the
 * traversal works and thus the status will be not supported.
 *
 * In the second case, we should be able to add a full set of announcements
 * that includes all combinations of severities and acknowledgements.  We
 * then will do the traversal and expect to see at least that many entries
 * in the annunciator.
 *****************************************************************************/

HpiTestStatus GetAll::runAnnunTest(SaHpiSessionIdT sessionId,
                                   SaHpiRptEntryT *rptEntry,
                                   SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    int count;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    HpiTestStatus writeStatus = setWriteMode(sessionId, resourceId, annunRec);
    if (writeStatus.hasFault()) {
        status.add(TRACE, writeStatus);
    } else  if (writeStatus.isNotSupported()) {
        status.add(TRACE, getAnnouncementCount(sessionId, resourceId, annunNum,
                                               SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                               &count));
        if (status.isOkay() && count > 0) {
            status.assertPass();
        } 
    } else {
        status.add(TRACE, addTestAnnouncements(sessionId, resourceId, annunNum));
        if (status.isOkay()) {
            status.add(TRACE, getAnnouncementCount(sessionId, resourceId, annunNum,
                                                   SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                                   &count));
            if (status.isOkay()){
                if (count >= (MAX_SEVERITIES * 2)) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, "Failed to read all of the announcements.");
                }
            }
        }

        status.add(TRACE, deleteTestAnnouncements(sessionId, resourceId, annunNum));
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

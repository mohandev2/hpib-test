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

#include "GetAnnouncement.h"
#include "Report.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAnnouncement::GetAnnouncement(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAnnouncement::getName() {
    return "GetAnnouncement";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAnnouncement::getDescription() {
    return "Individually retrieve all of the announcements in the Annunciator\n"
           "using saHpiAnnunciatorGet().  If the Annunciator is empty, try\n"
           "adding an announcement and retrieving it.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetAnnouncement::getPrecondition() {
    return "Requires an Annunciator with announcements or an Annunciator\n"
           "for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAnnouncement::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Try to test the get function by traversing the current announcements in
 * the Annunciator.  If the Annunciator is empty, then try adding an 
 * announcement and reading it back.
 *****************************************************************************/

HpiTestStatus GetAnnouncement::runAnnunTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    HpiTestStatus response = traversalTest(sessionId, resourceId, annunNum);
    if (!response.isNotSupported()) {
        status.add(TRACE, response);
    } else {
        status.add(TRACE, simpleTest(sessionId, resourceId, annunRec));
    }

    return status;
}

/*****************************************************************************
 * For each announcement in the Annunciator, retrieve it using
 * saHpiAnnunciatorGet() and verify that it is the same announcement
 * that was retrieved with the GetNext function.
 *****************************************************************************/

HpiTestStatus GetAnnouncement::traversalTest(SaHpiSessionIdT sessionId,
                                             SaHpiResourceIdT resourceId,
                                             SaHpiAnnunciatorNumT annunNum) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;
    SaHpiAnnouncementT getAnnouncement;
    Report report;

    status.assertNotSupport();
    announcement.EntryId = SAHPI_FIRST_ENTRY;
    while (!status.hasFault()) {
        SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum,
                                                 SAHPI_ALL_SEVERITIES, SAHPI_FALSE, 
                                                 &announcement);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, error);
        } else {
            error = saHpiAnnunciatorGet(sessionId, resourceId, annunNum,
                                        announcement.EntryId, &getAnnouncement);
            if (error != SA_OK) {
                status.assertFailure(TRACE, ANNUNCIATOR_GET, SA_OK, error);
            } else if (!AnnunciatorHelper::isEqual(&getAnnouncement, &announcement, report)) {
                status.assertFailure(TRACE, 
                            "Announcements do not match.\n%s", report.toString());
            } else {
                status.assertPass();
            }
        }
    }

    return status;
}

/*****************************************************************************
 * If possible, add an informational announcement and then retrieve it.
 *****************************************************************************/

HpiTestStatus GetAnnouncement::simpleTest(SaHpiSessionIdT sessionId,
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
                                                 announcement.EntryId, &announcement);
            if (error == SA_OK) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, ANNUNCIATOR_GET, SA_OK, error);
            }

            status.add(TRACE, deleteAnnouncement(sessionId, resourceId,
                                                 annunNum, &announcement));
        }
        status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

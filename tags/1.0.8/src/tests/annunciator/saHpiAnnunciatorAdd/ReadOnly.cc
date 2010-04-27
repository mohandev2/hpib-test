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

#include "ReadOnly.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnly::ReadOnly(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnly::getName() {
    return "ReadOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnly::getDescription() {
    return "The Annunciator is in AUTO mode and thus it is not "
           "possible to add an announcement.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires an Annunciator in AUTO mode or can be placed into AUTO mode.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnly::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReadOnly::runAnnunTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnunciatorModeT mode;

    SaErrorT error = saHpiAnnunciatorModeGet(sessionId, resourceId, annunNum, &mode);
    if (error != SA_OK) {
        status.assertError(TRACE, ANNUNCIATOR_MODE_GET, SA_OK, error);
    } else if (mode == SAHPI_ANNUNCIATOR_MODE_AUTO) {
        status.add(TRACE, testAnnunciator(sessionId, rptEntry, annunRec));
    } else if (annunRec->ModeReadOnly) {
        status.assertNotSupport();
    } else {
        error = saHpiAnnunciatorModeSet(sessionId, resourceId,
                                        annunNum, SAHPI_ANNUNCIATOR_MODE_AUTO);
        if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_MODE_SET, SA_OK, error);
        } else {
            status.add(TRACE, testAnnunciator(sessionId, rptEntry, annunRec));

            error = saHpiAnnunciatorModeSet(sessionId, resourceId, annunNum, mode);
            status.checkError(TRACE, ANNUNCIATOR_MODE_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Try to add an announcement to a read only annunciator.
 *****************************************************************************/

HpiTestStatus ReadOnly::testAnnunciator(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    AnnunciatorHelper::fill(&announcement);
    SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
                                         annunNum, &announcement);
    if (error == SA_ERR_HPI_READ_ONLY) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, ANNUNCIATOR_ADD,
                             SA_ERR_HPI_READ_ONLY, error);
    }

    return status;
}

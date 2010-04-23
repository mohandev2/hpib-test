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

#include "AnnouncementNotPresent.h"

using namespace ns_saHpiAnnunciatorAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AnnouncementNotPresent::AnnouncementNotPresent(char *line)
: AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AnnouncementNotPresent::getName() {
    return "AnnouncementNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AnnouncementNotPresent::getDescription() {
    return "An announcement identified by the <i>EntryId</i> parameter\n"
           "does not exist in the current set.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AnnouncementNotPresent::getPrecondition() {
    return "Requires an Annunciator.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AnnouncementNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AnnouncementNotPresent::runAnnunTest(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry,
                                                   SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    SaErrorT error = saHpiAnnunciatorAcknowledge(sessionId,
                                                 resourceId, annunNum, 
                                                 getInvalidAnnouncementId(),
                                                 SAHPI_ALL_SEVERITIES);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, ANNUNCIATOR_ACKNOWLEDGE,
                             SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}


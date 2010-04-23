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

using namespace ns_saHpiAnnunciatorModeSet;

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
    return "Changing the mode is not permitted for a read-only Annunciator.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires an Annunciator that is in Read Only mode.";
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

    if (!annunRec->ModeReadOnly) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAnnunciatorModeSet(sessionId,
                                                 resourceId, annunNum,
                                                 SAHPI_ANNUNCIATOR_MODE_SHARED);
        if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_MODE_SET,
                                 SA_ERR_HPI_READ_ONLY, error);
        }
    }

    return status;
}

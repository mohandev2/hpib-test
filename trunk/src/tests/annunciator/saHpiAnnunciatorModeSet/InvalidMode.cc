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

#include "InvalidMode.h"

using namespace ns_saHpiAnnunciatorModeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidMode::InvalidMode(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidMode::getName() {
    return "InvalidMode";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidMode::getDescription() {
    return "The <i>Mode</i> parameter is not one of the valid enumerated values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidMode::getPrecondition() {
    return "Requires an Annunciator that is not in Read Only mode.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidMode::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidMode::runAnnunTest(SaHpiSessionIdT sessionId,
                                        SaHpiRptEntryT *rptEntry,
                                        SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    int invalidMode = (int) SAHPI_ANNUNCIATOR_MODE_SHARED + 1;

    if (annunRec->ModeReadOnly) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAnnunciatorModeSet(sessionId, resourceId, annunNum,
                                                 (SaHpiAnnunciatorModeT) invalidMode);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_MODE_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

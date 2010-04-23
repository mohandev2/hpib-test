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

#include "GetMode.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorModeGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetMode::GetMode(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetMode::getName() {
    return "GetMode";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetMode::getDescription() {
    return "Retrieve the current operating mode of an Annunciator.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetMode::getPrecondition() {
    return "Requires an Annunciator.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetMode::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetMode::runAnnunTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnunciatorModeT mode;

    SaErrorT error = saHpiAnnunciatorModeGet(sessionId, resourceId, annunNum, &mode);
    if (error != SA_OK) {
        status.assertFailure(TRACE, ANNUNCIATOR_MODE_GET, SA_OK, error);
    } else if (!AnnunciatorHelper::isValidMode(mode)) {
        status.assertFailure(TRACE, "Annunciator mode is invalid: %d.", mode);
    } else {
        status.assertPass();
    }

    return status;
}

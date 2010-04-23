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

#include "SetMode.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorModeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetMode::SetMode(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetMode::getName() {
    return "SetMode";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetMode::getDescription() {
    return "Change the current operating mode of an Annunciator.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetMode::getPrecondition() {
    return "Requires an Annunciator that is not in Read Only mode.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetMode::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetMode::runAnnunTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnunciatorModeT origMode;
    SaHpiAnnunciatorModeT mode[] = { SAHPI_ANNUNCIATOR_MODE_AUTO,
                                     SAHPI_ANNUNCIATOR_MODE_USER,
                                     SAHPI_ANNUNCIATOR_MODE_SHARED };

    if (annunRec->ModeReadOnly) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAnnunciatorModeGet(sessionId, resourceId,
                                                 annunNum, &origMode);
        if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_MODE_GET, SA_OK, error);
        } else {
            for (int i = 0; i < 3; i++) {
                error = saHpiAnnunciatorModeSet(sessionId, resourceId,
                                                annunNum, mode[i]);
                if (error == SA_OK) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, ANNUNCIATOR_MODE_SET, SA_OK, error);
                }
            }

            error = saHpiAnnunciatorModeSet(sessionId, resourceId, annunNum, origMode);
            status.checkError(TRACE, ANNUNCIATOR_MODE_SET, error);
        }
    }

    return status;
}

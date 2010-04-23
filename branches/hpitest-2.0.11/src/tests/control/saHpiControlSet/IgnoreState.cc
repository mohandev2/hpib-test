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

#include "IgnoreState.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IgnoreState::IgnoreState(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *IgnoreState::getName() {
    return "IgnoreState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *IgnoreState::getDescription() {
    return "If the <i>CtrlMode</i> parameter is set to SAHPI_CTRL_MODE_AUTO,\n"
           "the <i>CtrlState</i> parameter is not evaluated, and may be set\n"
           "to any value by an HPI User, including a NULL pointer.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *IgnoreState::getPrecondition() {
    return "Requires a Control that is in AUTO mode or that can "
           "be placed into AUTO mode.  If necessary to change a control "
           "to AUTO mode, the control must also be readable.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT IgnoreState::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus IgnoreState::runCtrlTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;

    // If the default mode is AUTO and the mode is read-only, i.e. it can't
    // be changed.  Therefore, the current mode must be AUTO and we can just
    // run the test.

    if (getDefaultMode(ctrlRec) == SAHPI_CTRL_MODE_AUTO && isReadOnlyMode(ctrlRec)) {
        status.add(TRACE, runIgnoreTest(sessionId, rptEntry->ResourceId, ctrlRec));

    } else if (!isReadOnlyMode(ctrlRec) && !ctrlRec->WriteOnly) {

        // The mode can be changed and we can read the current state.
        // If the mode is already AUTO, just run the test.  If the mode is
        // MANUAL, save the CtrlState before running the test.  We must
        // restore the mode and the CtrlState after the test.

        SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &ctrlMode, NULL);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else if (ctrlMode == SAHPI_CTRL_MODE_AUTO) {
            status.add(TRACE, runIgnoreTest(sessionId, rptEntry->ResourceId, ctrlRec));
        } else {
            status.add(TRACE, saveControlData(sessionId, rptEntry->ResourceId, ctrlRec));
            if (status.isOkay()) {
                status.add(TRACE, runIgnoreTest(sessionId, rptEntry->ResourceId, ctrlRec));
                status.add(TRACE, restoreControlData(sessionId,
                                                     rptEntry->ResourceId,
                                                     ctrlRec->Num));
            }
        }
    } else {
        status.assertNotSupport();
    }

    return status;
}

/*****************************************************************************
 * When setting the mode to AUTO, the CtrlState is ignored.  Test two cases:
 * the CtrlState->Type is invalid and, secondly, the CtrlState is NULL.  
 *****************************************************************************/

HpiTestStatus IgnoreState::runIgnoreTest(SaHpiSessionIdT sessionId,
                                         SaHpiResourceIdT resourceId,
                                         SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;
    int invalidCtrlType = (int) SAHPI_CTRL_TYPE_TEXT + 1;

    setDefaultCtrlState(ctrlRec, &ctrlState);
    ctrlState.Type =  (SaHpiCtrlTypeT) invalidCtrlType;

    SaErrorT error = saHpiControlSet(sessionId, resourceId, ctrlRec->Num,
                                     SAHPI_CTRL_MODE_AUTO, &ctrlState);

    if ((error == SA_ERR_HPI_INVALID_DATA) ||
        (error == SA_ERR_HPI_INVALID_REQUEST)) {
        status.assertNotSupport();

    } else if (error != SA_OK) {
        status.assertFailure(TRACE, CONTROL_SET, SA_OK, error,
                             "Using an invalid CtrlState->Type.");
    }

    error = saHpiControlSet(sessionId, resourceId, ctrlRec->Num,
                            SAHPI_CTRL_MODE_AUTO, NULL);

    if ((error == SA_ERR_HPI_INVALID_DATA) ||
        (error == SA_ERR_HPI_INVALID_REQUEST)) {
        status.assertNotSupport();

    } else if (error != SA_OK) {
        status.assertFailure(TRACE, CONTROL_SET, SA_OK, error,
                             "Using a NULL pointer for CtrlState.");
    }

    if (status.isOkay()) {
        status.assertPass();
    }

    return status;
}


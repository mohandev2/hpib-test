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

#include "NullParams.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullParams::NullParams(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullParams::getName() {
    return "NullParams";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullParams::getDescription() {
    return "The <i>CtrlMode</i> and <i>CtrlState</i> pointers are "
           "passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullParams::getPrecondition() {
    return "Requires a readable Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullParams::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Try all combinations of NULL pointers.
 *****************************************************************************/

HpiTestStatus NullParams::runCtrlTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;

    if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, testControl(sessionId, rptEntry,
                                      ctrlRec, &ctrlMode, NULL));

        if (isTextControl(ctrlRec)) {
            ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
            ctrlState.StateUnion.Text.Line = 1;
        }

        status.add(TRACE, testControl(sessionId, rptEntry,
                                      ctrlRec, NULL, &ctrlState));

        status.add(TRACE, testControl(sessionId, rptEntry,
                                      ctrlRec, NULL, NULL));
    }

    return status;
}

/*****************************************************************************
 * Get the control's mode and state.
 *****************************************************************************/

HpiTestStatus NullParams::testControl(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec,
                                      SaHpiCtrlModeT *ctrlMode,
                                      SaHpiCtrlStateT *ctrlState) {
    HpiTestStatus status;

    SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                     ctrlRec->Num, ctrlMode, ctrlState);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, CONTROL_GET, SA_OK, error);
    } 

    return status;
}

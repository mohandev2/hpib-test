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

#include "GetControl.h"
#include "ControlHelper.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetControl::GetControl(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetControl::getName() {
    return "GetControl";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetControl::getDescription() {
    return "Read the <i>CtrlMode</i> and <i>CtrlState</i> and verify that\n"
           "the <i>CtrlMode</i> and the <i>CtrlState.Type</i> are legal.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetControl::getPrecondition() {
    return "Requires a readable Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetControl::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetControl::runCtrlTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT mode;
    SaHpiCtrlStateT state;

    if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else {
        if (isTextControl(ctrlRec)) {
            state.Type = SAHPI_CTRL_TYPE_TEXT;
            state.StateUnion.Text.Line = 1;
        }
        SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &mode, &state);
        if (error != SA_OK) {
            status.assertFailure(TRACE, CONTROL_GET, SA_OK, error);
        } else {
            status.assertPass();

            if (!ControlHelper::isValidCtrlMode(mode)) {
                status.assertFailure(TRACE, "Control Mode is invalid: %d", mode);
            }
    
            if (!ControlHelper::isValidCtrlType(state.Type)) {
                status.assertFailure(TRACE, "Control Type is invalid: %d", state.Type);
            }
        }
    }

    return status;
}

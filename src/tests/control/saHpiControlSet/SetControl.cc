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

#include "SetControl.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetControl::SetControl(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetControl::getName() {
    return "SetControl";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetControl::getDescription() {
    return "For a control, read it's current <i>CtrlMode</i> and\n"
           "<i>CtrlState</i> and write it back as is.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetControl::getPrecondition() {
    return "Requires a readable Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetControl::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetControl::runCtrlTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;

    if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else {
        if (isTextControl(ctrlRec)) {
            ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
            ctrlState.StateUnion.Text.Line = 1;
        }

        SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &ctrlMode, &ctrlState);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else {
            error = saHpiControlSet(sessionId, rptEntry->ResourceId,
                                    ctrlRec->Num, ctrlMode, &ctrlState);
            if (error == SA_OK) 
            {
	      status.assertPass();

	      //B.02 code change
	    } else if ((error == SA_ERR_HPI_INVALID_DATA) && 
		       (ctrlState.Type == SAHPI_CTRL_TYPE_OEM)) {
	      status.assertNotSupport();

	    } else if (error == SA_ERR_HPI_UNSUPPORTED_PARAMS) {
	      status.assertNotSupport();

            } else {
                status.assertFailure(TRACE, CONTROL_SET, SA_OK, error);
            }
        }
    }

    return status;
}

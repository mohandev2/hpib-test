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

#include "InvalidLineNumber.h"
#include "ControlHelper.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidLineNumber::InvalidLineNumber(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidLineNumber::getName() {
    return "InvalidLineNumber";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidLineNumber::getDescription() {
    return "For a text control, pass in a line number that does not exist.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidLineNumber::getPrecondition() {
    return "Requires a readable Text Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidLineNumber::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidLineNumber::runCtrlTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;

    if (ctrlRec->WriteOnly || !isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else {
        ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
        ctrlState.StateUnion.Text.Line = ctrlRec->TypeUnion.Text.MaxLines + 1;

        SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &ctrlMode, &ctrlState);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, CONTROL_GET,
                                 SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}

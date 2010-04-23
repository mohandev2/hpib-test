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

#include "ClearLine.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ClearLine::ClearLine(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ClearLine::getName() {
    return "ClearLine";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ClearLine::getDescription() {
    return "Clear the first line by writing zero bytes to it.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ClearLine::getPrecondition() {
    return "Requires a readable Text Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ClearLine::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Clear the first line and verify that it is cleared.
 *****************************************************************************/

HpiTestStatus ClearLine::runCtrlTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT origCtrlMode;
    SaHpiCtrlStateT origCtrlState;
    SaHpiCtrlStateT ctrlState;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    int maxChars = getMaxChars(ctrlRec);

    if (ctrlRec->WriteOnly || !isTextControl(ctrlRec) || !canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {

        // Save the original mode and state data so that it can be restored.
        origCtrlState.Type = SAHPI_CTRL_TYPE_TEXT;
        origCtrlState.StateUnion.Text.Line = 1;
        SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                         ctrlNum, &origCtrlMode, &origCtrlState);

        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else {

            // Write the initial string to make sure we have something here.

            status.add(TRACE, setControlTextBuffer(sessionId, resourceId,
                                                   ctrlRec, 1, maxChars / 2, BYTE_VALUE_1));
            if (status.isOkay()) {

                // Write a zero-length empty string.

                status.add(TRACE, setControlTextBuffer(sessionId, resourceId, ctrlRec, 1, 0, 0));
                if (status.isOkay()) {

                    // Retrieve the latest text buffer and verify that it
                    // corresponds to the empty string.
                    ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
                    ctrlState.StateUnion.Text.Line = 1;
                    error = saHpiControlGet(sessionId, resourceId, ctrlNum, NULL, &ctrlState);
                    if ((error == SA_ERR_HPI_INVALID_DATA) ||
                        (error == SA_ERR_HPI_INVALID_REQUEST)) {
                        status.assertNotSupport();
                    } else if (error != SA_OK) {
                        status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                    } else {
                        CharBuffer buffer(ctrlState.StateUnion.Text.Text);
                        if (!buffer.isBlank(maxChars)) {
                            status.assertFailure(TRACE, "Text line 1 does not contain blanks!");
                        } else {
                            status.assertPass();
                        }
                    }
                }

                // Restore the original mode and state data.

                error = saHpiControlSet(sessionId, resourceId, ctrlNum,
                                        origCtrlMode, &origCtrlState);
                status.checkError(TRACE, CONTROL_SET, error);
            }
        }
    }

    return status;
}

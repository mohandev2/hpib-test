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

#include "ClearAllLines.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ClearAllLines::ClearAllLines(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ClearAllLines::getName() {
    return "ClearAllLines";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ClearAllLines::getDescription() {
    return "Clear the entire text control by using SAHPI_TLN_ALL_LINES\n"
           "as the line number and writing zero bytes.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ClearAllLines::getPrecondition() {
    return "Requires a readable Text Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ClearAllLines::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * In a text control, clear all of the text buffers with SAHPI_TLN_ALL_LINES
 * and verify that all of the lines have been cleared.
 *****************************************************************************/

HpiTestStatus ClearAllLines::runCtrlTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;

    if (ctrlRec->WriteOnly || !isTextControl(ctrlRec) || 
        !canSetControlState(ctrlRec)) {

        status.assertNotSupport();
    } else {

        // Read the mode and all of the text buffers for later restoration.

        status.add(TRACE, saveControlData(sessionId, resourceId, ctrlRec));
        if (status.isOkay()) {

            // Fill up the text buffers so that we know we really have something.

            status.add(TRACE, setControlAllTextBuffers(sessionId, resourceId,
                                                       ctrlRec, BYTE_VALUE_1));
            if (status.isOkay()) {

                // Clear all of the text buffers.

                status.add(TRACE, setControlTextBuffer(sessionId, resourceId,
                                                       ctrlRec, SAHPI_TLN_ALL_LINES,
                                                       0, 0));
                if (status.isOkay()) {

                    // Verify that the text buffers contain what we expect.

                    status.add(TRACE, verifyTextBuffers(sessionId, resourceId, ctrlRec));
                }

                status.add(TRACE, restoreControlData(sessionId, resourceId, ctrlNum));
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Verify that all of the text buffers are blank.
 *****************************************************************************/

HpiTestStatus ClearAllLines::verifyTextBuffers(SaHpiSessionIdT sessionId,
                                                  SaHpiResourceIdT resourceId,
                                               SaHpiCtrlRecT * ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiCtrlStateT ctrlState;
    int maxLines = getMaxLines(ctrlRec);
    int maxChars = getMaxChars(ctrlRec);

    // Verify that all of the lines are empty.

    status.assertPass();
    for (int lineNum = 1; lineNum <= maxLines && status.isOkay(); lineNum++) {
        ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
        ctrlState.StateUnion.Text.Line = lineNum;
        SaErrorT error = saHpiControlGet(sessionId, resourceId, ctrlNum, NULL, &ctrlState);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else {
            CharBuffer buffer(ctrlState.StateUnion.Text.Text);
            if (!buffer.isBlank(maxChars)) {
                status.assertFailure(TRACE, "Text line %d is not blank!", lineNum);
            }
        }
    }

    return status;
}

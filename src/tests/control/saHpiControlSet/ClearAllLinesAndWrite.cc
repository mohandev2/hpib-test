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

#include "ClearAllLinesAndWrite.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ClearAllLinesAndWrite::ClearAllLinesAndWrite(char *line)
: ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ClearAllLinesAndWrite::getName() {
    return "ClearAllLinesAndWrite";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ClearAllLinesAndWrite::getDescription() {
    return "When calling saHpiControlSet() for a text control, an HPI User\n"
           "may set the line number to SAHPI_TLN_ALL_LINES; in this case,\n"
           "the entire control will be cleared, and the data will be written\n"
           "starting on ine 1.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ClearAllLinesAndWrite::getPrecondition() {
    return "Requires a readable Text Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ClearAllLinesAndWrite::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * When writing to a text buffer using SAHPI_TLN_ALL_LINES, the text buffer
 * will be cleared and the text will be written starting at line 1.  In
 * this test, only half a line will be written to line 1.  The other lines
 * should all be cleared.  After writing, verify that the change occurred
 * correctly.
 *****************************************************************************/

HpiTestStatus ClearAllLinesAndWrite::runCtrlTest(SaHpiSessionIdT sessionId,
                                                 SaHpiRptEntryT *rptEntry,
                                                 SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;

    if (ctrlRec->WriteOnly ||
        !isTextControl(ctrlRec) || !canSetControlState(ctrlRec)) {

        status.assertNotSupport();
    } else {

        // Read the mode and all of the text buffers for later restoration.

        status.add(TRACE, saveControlData(sessionId, resourceId, ctrlRec));
        if (status.isOkay()) {

            // Fill up the text buffers so that we know we 
            // really have something to clear.

            status.add(TRACE, setControlAllTextBuffers(sessionId, resourceId,
                                                       ctrlRec, BYTE_VALUE_1));
            if (status.isOkay()) {

                // Clear all of the text buffers and 
                // overwrite half of the first line.

                int numChars = getMaxChars(ctrlRec) / 2;
                status.add(TRACE, setControlTextBuffer(sessionId, resourceId,
                                                       ctrlRec, SAHPI_TLN_ALL_LINES,
                                                       numChars, BYTE_VALUE_2));
                if (status.isOkay()) {

                    // Verify that the text buffers contain what we expect.

                    status.add(TRACE, verifyTextBuffers(sessionId, resourceId,
                                                        ctrlRec, numChars));
                }
            }

            // Restore the mode and text data.

            status.add(TRACE, restoreControlData(sessionId, resourceId, ctrlNum));
        }
    }

    return status;
}

/*****************************************************************************
 * Verify the text in the text buffers.  The first line should be half
 * filled with BYTE_VALUE_2 and the remaining lines should be blank.
 *****************************************************************************/

HpiTestStatus ClearAllLinesAndWrite::verifyTextBuffers(SaHpiSessionIdT sessionId,
                                                       SaHpiResourceIdT resourceId,
                                                       SaHpiCtrlRecT * ctrlRec,
                                                       int numCharsFirstLine) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    int maxLines = getMaxLines(ctrlRec);
    int maxChars = getMaxChars(ctrlRec);

    // Get the first text buffer line.

    ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
    ctrlState.StateUnion.Text.Line = 1;
    SaErrorT error = saHpiControlGet(sessionId, resourceId, ctrlNum, NULL, &ctrlState);
    if (error != SA_OK) {
        status.assertError(TRACE, CONTROL_GET, SA_OK, error);
    } else {

        // Verify that the text buffer matches what we expect.
        CharBuffer buffer(ctrlState.StateUnion.Text.Text);
        if (!buffer.matches(BYTE_VALUE_2, numCharsFirstLine)) {
            status.assertFailure(TRACE, "First text line does not have the expected text.");
        } else {

            // Verify that all of the other lines are blank.

            status.assertPass();
            for (int lineNum = 2; lineNum <= maxLines && status.isOkay(); lineNum++) {
                ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
                ctrlState.StateUnion.Text.Line = lineNum;
                error = saHpiControlGet(sessionId, resourceId, ctrlNum, NULL, &ctrlState);
                if (error != SA_OK) {
                    status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                } else {
                    CharBuffer buffer2(ctrlState.StateUnion.Text.Text);
                    if (!buffer2.isBlank(maxChars)) {
                        status.assertFailure(TRACE, "Text line %d is not blank!", lineNum);
                    }
                }
            }
        }
    }

    return status;
}


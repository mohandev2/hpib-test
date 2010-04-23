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

#include "WrapText.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WrapText::WrapText(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *WrapText::getName() {
    return "WrapText";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *WrapText::getDescription() {
    return "For a text control, if the text in the <i>CtrlState</i> structure\n"
           "is more than will fit on one line, the control will wrap the text\n"
           "to the next line.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *WrapText::getPrecondition() {
    return "Requires a readable Text Control whose state can be set "
           "and for which a line wrap is possible.  In other words, a "
           "SaHpiTextBufferT can hold more characters than get fit on "
           "single line in the Text Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT WrapText::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Write to line 1 more text than will fit one line 1.  The text should wrap
 * to line 2.  
 *****************************************************************************/

HpiTestStatus WrapText::runCtrlTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    int maxChars = getMaxChars(ctrlRec);

    if (ctrlRec->WriteOnly || !isTextControl(ctrlRec) || 
        !canSetControlState(ctrlRec) || !canWrap(ctrlRec)) {

        status.assertNotSupport();

    } else {

        // Save the original mode and state data so that it can be restored.

        status.add(TRACE, saveControlData(sessionId, resourceId, ctrlRec));

        if (status.isOkay()) {

            // Write a text string into the first line that will overflow 
            // into the second line, but which will not fill up the second line.
            // We don't want to overflow into the third line since there might 
            // not be one.  Therefore, we may have to limit how much we write.
            // Also, we might not be able to put enough into one buffer to fill 
            // both lines.  So, we must compute how many characters are actually
            // written to the first and second lines.

            int numChars = (int) (maxChars * 1.5);

            int numChars_1 = maxChars;
            int numChars_2 = numChars - numChars_1;

            status.add(TRACE, setControlTextBuffer(sessionId, resourceId,
                                                   ctrlRec, 1, numChars, BYTE_VALUE_1));
            if (status.isOkay()) {

                // Verify that the first and second lines have the data that is expected.

                status.add(TRACE, verifyLine(sessionId, resourceId,
                                             ctrlRec, 1, numChars_1, BYTE_VALUE_1));
                if (status.isOkay()) {
                    status.add(TRACE, verifyLine(sessionId, resourceId,
                                                 ctrlRec, 2, numChars_2, BYTE_VALUE_1));
                }
            }

            // Restore the original mode and state data.
            status.add(TRACE, restoreControlData(sessionId, resourceId, ctrlNum));
        }
    }

    return status;
}

/*****************************************************************************
 * Can we wrap the text?  We need at least 2 lines in the text control and
 * we need to be able to write more than will fit on a single line.
 *****************************************************************************/

bool WrapText::canWrap(SaHpiCtrlRecT * ctrlRec) {
    return (ctrlRec->TypeUnion.Text.MaxLines >= 2) && (canOverflowLine(ctrlRec));
}

/*****************************************************************************
 * Verify that specified lineNum has the specified sequence of characters.
 *****************************************************************************/

HpiTestStatus WrapText::verifyLine(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId,
                                   SaHpiCtrlRecT * ctrlRec,
                                   SaHpiTxtLineNumT lineNum,
                                   SaHpiUint8T numChars,
                                   SaHpiUint8T byteValue) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;

    ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
    ctrlState.StateUnion.Text.Line = lineNum;
    SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                     ctrlNum, NULL, &ctrlState);

    if (error != SA_OK) {
        status.assertError(TRACE, CONTROL_GET, SA_OK, error);
    } else {
        CharBuffer buffer(ctrlState.StateUnion.Text.Text);
        if (buffer.matches(byteValue, numChars)) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, 
                "Text line %d does not have the expected characters.", lineNum);
        }
    }

    return status;
}

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

#include "SingleLine.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SingleLine::SingleLine(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SingleLine::getName() {
    return "SingleLine";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SingleLine::getDescription() {
    return "For a text control, set each single line\n"
           "and verify that each line was set correctly.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SingleLine::getPrecondition() {
    return "Requires a readable Text Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SingleLine::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Fill each line of the text buffer and then verify that the
 * change worked correctly.
 *****************************************************************************/

HpiTestStatus SingleLine::runCtrlTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;

    if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else if (!isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, saveControlData(sessionId, resourceId, ctrlRec));
        if (status.isOkay()) {

            int maxLines = getMaxLines(ctrlRec);
            int maxChars = getMaxChars(ctrlRec);

            // Initialize the ctrl state information that is the same
            // for each invocation of saHpiControlSet().

            status.add(TRACE, setControlAllTextBuffers(sessionId, resourceId,
                                                       ctrlRec, BYTE_VALUE_1));
            if (status.isOkay()) {

                // If we succesfully change each line of text, then read the
                // text data to verify that the change really did occur.

                status.assertPass();
                for (int lineNum = 1; lineNum <= maxLines && status.isOkay(); lineNum++) {

                    ctrlState.StateUnion.Text.Line = lineNum;
                    SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                                     ctrlNum, &ctrlMode, &ctrlState);
                    if (error != SA_OK) {
                        status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                    } else {
                        CharBuffer buffer(ctrlState.StateUnion.Text.Text);
                        if (!buffer.matches(BYTE_VALUE_1, maxChars)) {
                            status.assertFailure(TRACE, "Text line %d has unexpected characters.", lineNum);
                        }
                    }
                }
            }

            status.add(TRACE, restoreControlData(sessionId, resourceId, ctrlNum));
        }
    }

    return status;
}

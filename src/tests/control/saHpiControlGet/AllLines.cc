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

#include "AllLines.h"
#include "ControlHelper.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AllLines::AllLines(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AllLines::getName() {
    return "AllLines";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AllLines::getDescription() {
    return "If the line number passed in is SAHPI_TLN_ALL_LINES, then\n"
           "saHpiControlGet() willl return the entire text of the control\n"
           "or as much as will fit in a single text buffer.  To verify\n"
           "the operation, the lines will also be read individually and\n"
           "cross-checked.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AllLines::getPrecondition() {
    return "Requires a readable Text Control that is or can "
           "be put into MANUAL mode.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AllLines::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * If we have a Text control that we can read, then try reading all of
 * the text data via TLN_ALL_LINES.  If successful, verify that the text
 * data corresponds to reading the data one line at a time.
 *
 * This test will be limited to controls in MANUAL mode.  This is because
 * we can get a race condition in AUTO mode.  For example, after getting
 * all of the lines in one call, the text lines could then be automatically
 * changed.  The test would then fail since the data has changed.
 *****************************************************************************/

HpiTestStatus AllLines::runCtrlTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;

    if (!isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else if (getDefaultMode(ctrlRec) == SAHPI_CTRL_MODE_AUTO && isReadOnlyMode(ctrlRec)) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, saveControlData(sessionId, rptEntry->ResourceId, ctrlRec));
        if (status.isOkay()) {
            status.add(TRACE, setupTestData(sessionId, rptEntry->ResourceId, ctrlRec));
            if (status.isOkay()) {
                status.add(TRACE, runAllLinesTest(sessionId, rptEntry->ResourceId, ctrlRec));
            }
            status.add(TRACE, restoreControlData(sessionId, rptEntry->ResourceId, ctrlRec->Num));
        }
    }

    return status;
}

/*****************************************************************************
 * Only fill up half of each line so that we can get some blanks.
 *****************************************************************************/

HpiTestStatus AllLines::setupTestData(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiCtrlRecT * ctrlRec) {
    HpiTestStatus status;
    int maxLines = getMaxLines(ctrlRec);
    int numChars = getMaxChars(ctrlRec) / 2;

    for (int lineNum = 1; lineNum <= maxLines; lineNum++) {
        status.add(TRACE, setControlTextBuffer(sessionId, resourceId, ctrlRec,
                                               lineNum, numChars, BYTE_VALUE_1));
        if (!status.isOkay()) {
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * Verify that the Text data obtained via TLN_ALL_LINES corresponds to
 * the text data that can be obtained by reading one line at a time.
 *
 * This algorithm is tricky.  First, consider what happens when lines are
 * concatenated for TLN_ALL_LINES.  Trailing blanks are not removed.  For
 * example, if the MaxChars per line is 100, but a line only has 10 chars,
 * there will be 90 blanks in the text buffer BEFORE the next line that
 * is concatenated on.
 *
 * Secondly, because a text buffer can only store 255 bytes, it may be
 * that the last line that is concatenated is trimmed.
 *
 * Because UNICODE requires 2 bytes per character, the below algorithm
 * uses a CharBuffer in order to process characters rather than bytes.
 *
 * The below algorithm takes the approach of comparing a each individual
 * line of text data to a portion of the ALL LINES text data.  The "nextIndex"
 * is the index of where the comparison must begin.  Note that "nextIndex"
 * is incremented by "maxChars" to skip over blanks.
 *
 *****************************************************************************/

HpiTestStatus AllLines::runAllLinesTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiCtrlRecT * ctrlRec) {
    HpiTestStatus status;
    int lineNum = 1;
    int nextIndex = 0;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlStateT ctrlStateAllLines;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    int maxChars = getMaxChars(ctrlRec);

    ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
    ctrlStateAllLines.StateUnion.Text.Line = SAHPI_TLN_ALL_LINES;
    SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                     ctrlRec->Num, &ctrlMode,
                                     &ctrlStateAllLines);
    if (error != SA_OK) {
        status.assertFailure(TRACE, CONTROL_GET, SA_OK, error);
    } else {
        CharBuffer allLinesBuffer(ctrlStateAllLines.StateUnion.Text.Text);
        SaHpiUint8T charCnt = allLinesBuffer.getLength();

        // If we don't fill up a Text Buffer, then we must have read all
        // of the text data from all of the lines.  If we didn't read all
        // the characters (maxPossibleChars), this is an error.

        int maxPossibleChars = maxChars * getMaxLines(ctrlRec);
        SaHpiUint8T maxCharsPerBuffer = getMaxCharsPerBuffer(ctrlRec);
        if ((charCnt < maxCharsPerBuffer) && (charCnt != maxPossibleChars)) {
            status.assertFailure(TRACE, "Did not fill Text Buffer when reading all lines!");
        } else {
            status.assertPass();
            while (status.isOkay() && nextIndex < allLinesBuffer.getLength()) {

                ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
                ctrlState.StateUnion.Text.Line = lineNum;
                error = saHpiControlGet(sessionId, resourceId,
                                        ctrlNum, &ctrlMode, &ctrlState);
                if (error != SA_OK) {
                    status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                } else {
                    CharBuffer singleLineBuffer(ctrlState.StateUnion.Text.Text);
                    SaHpiUint8T length = singleLineBuffer.getLength();

                    // If we last appended line was trimmed, then we must only
                    // match against the remaining characters in the All Lines Buffer.

                    if (length > charCnt) {
                        if (!allLinesBuffer.matches(singleLineBuffer, nextIndex, charCnt)) {
                            status.assertFailure(TRACE, "Text Buffers do not match!");
                        }

                    // We must match the entire single line against a part of
                    // of the All Lines Buffer starting at the given index.

                    } else if (!allLinesBuffer.matches(singleLineBuffer, nextIndex)) {
                        status.assertFailure(TRACE, "Text Buffers do not match!");

                    } else {
                        // If we got a short line due to blanks, be sure that we
                        // got blanks in the All Lines Text Buffer.

                        SaHpiUint8T numBlankChars = maxChars - length;
                        if (!allLinesBuffer.isBlank(nextIndex + length, numBlankChars)) {
                            status.assertFailure(TRACE, 
                                          "The All Lines TextBuffer is missing blanks!");
                        }
                    }
                }

                nextIndex += maxChars;
                lineNum++;
                charCnt -= maxChars;
            }
        }
    }

    return status;
}


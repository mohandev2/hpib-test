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

#include "ClearSpaces.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ClearSpaces::ClearSpaces(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ClearSpaces::getName() {
    return "ClearSpaces";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ClearSpaces::getDescription() {
    return "For a text control, if less than a full line of data is written,\n"
           "the control will clear all spaces beyond those written on the line.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ClearSpaces::getPrecondition() {
    return "Requires a readable Text Control whose state can be set and "
           "the MaxChars is four or greater.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ClearSpaces::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Save the control data before running the test.
 *****************************************************************************/

HpiTestStatus ClearSpaces::runCtrlTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;

    int maxChars = getMaxChars(ctrlRec);
    if (ctrlRec->WriteOnly || !isTextControl(ctrlRec) || 
        !canSetControlState(ctrlRec) || maxChars < 4) {

        status.assertNotSupport();
    } else {

        // Save the original mode and state data so that it can be restored.

        status.add(TRACE, saveControlData(sessionId,
                                          rptEntry->ResourceId,
                                          ctrlRec));
        if (status.isOkay()) {
            status.add(TRACE, runMyTest(sessionId,
                                        rptEntry->ResourceId,
                                        ctrlRec));

            // Restore the original mode and state data.
            status.add(TRACE, restoreControlData(sessionId,
                                                 rptEntry->ResourceId,
                                                 ctrlRec->Num));
        }
    }

    return status;
}

/*****************************************************************************
 * For each line, write a short line and verify that all of the remaining
 * spaces on the line are blank.
 *****************************************************************************/

HpiTestStatus ClearSpaces::runMyTest(SaHpiSessionIdT sessionId,
                                     SaHpiResourceIdT resourceId,
                                     SaHpiCtrlRecT * ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    int maxLines = getMaxLines(ctrlRec);
    int maxChars = getMaxChars(ctrlRec);
    SaHpiCtrlStateT ctrlState;

    int longLength = maxChars / 2;
    int shortLength = maxChars / 4;

    for (int lineNum = 1; lineNum <= maxLines && status.isOkay(); lineNum++) {

        // Write the initial "long" string in the first line.

        status.add(TRACE, setControlTextBuffer(sessionId, resourceId,
                                               ctrlRec, lineNum,
                                               longLength, BYTE_VALUE_1));
        if (status.isOkay()) {

            // Write a "shorter" string with a different character.

            status.add(TRACE, setControlTextBuffer(sessionId, resourceId,
                                                   ctrlRec, lineNum,
                                                   shortLength, BYTE_VALUE_2));
            if (status.isOkay()) {

                // Retrieve the latest text buffer and verify that it
                // corresponds to the "shorter" string.
                
                ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
                ctrlState.StateUnion.Text.Line = lineNum;
                SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                                 ctrlNum, NULL, &ctrlState);
                if (error != SA_OK) {
                    status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                } else {
                    CharBuffer buffer(ctrlState.StateUnion.Text.Text);
                    if (!buffer.matches(BYTE_VALUE_2, shortLength)) {
                        status.assertFailure(TRACE, 
                                "Text line %d does not match expected text.", lineNum);
                    }
                }
            }
        }
    }

    return status;
}


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

#include "MaxCharsLines.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

MaxCharsLines::MaxCharsLines(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *MaxCharsLines::getName() {
    return "MaxCharsLines";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *MaxCharsLines::getDescription() {
    return "For a text control, verify that the data conforms "
           "to the <i>MaxChars</i> and <i>MaxLines</i> as defined in the RDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *MaxCharsLines::getPrecondition() {
    return "Requires a readable Text Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT MaxCharsLines::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * For a text control, read all of the text data and verify that each line
 * doesn't exceed MaxChars and that we really do have MaxLines and that we 
 * cannot exceed MaxLines.
 *****************************************************************************/

HpiTestStatus MaxCharsLines::runCtrlTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;

    if (ctrlRec->WriteOnly || !isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else {

        int maxChars = ctrlRec->TypeUnion.Text.MaxChars;
        int maxLines = ctrlRec->TypeUnion.Text.MaxLines;

        // We should be able to read every text line.  If we can't, this
        // is obviously an error.  Also, the length of each line cannot
        // be greater than the maximum number of allowed characters.

        for (int line = 1; line <= maxLines; line++) {

            ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
            ctrlState.StateUnion.Text.Line = line;
            SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                             ctrlRec->Num, &ctrlMode, &ctrlState);

            if (error != SA_OK) {  // the line had better be there
                status.assertFailure(TRACE, CONTROL_GET, SA_OK, error);
                break;
            } else {
                CharBuffer buffer(ctrlState.StateUnion.Text.Text);
                if (buffer.getLength() > maxChars) {
                    status.assertFailure(TRACE, "Line %d has too many characters [%d]!", line,
                                            buffer.getLength());
                    break;
                }
            }
        }

        // If we haven't encountered any errors, try reading a line beyond
        // the "MaxLines" limit.  This should result in an error.

        if (status.isOkay()) {
            ctrlState.Type = SAHPI_CTRL_TYPE_TEXT;
            ctrlState.StateUnion.Text.Line = maxLines + 1;
            SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                             ctrlRec->Num, &ctrlMode, &ctrlState);
            if (error == SA_ERR_HPI_INVALID_DATA) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, CONTROL_GET,
                                     SA_ERR_HPI_INVALID_DATA, error);
            }
        }
    }

    return status;
}

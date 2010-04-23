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

#include "InvalidDataLength.h"
#include "CharBuffer.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidDataLength::InvalidDataLength(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidDataLength::getName() {
    return "InvalidDataLength";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidDataLength::getDescription() {
    return "<i>CtrlState->StateUnion.Text.Text.DataLength</i>, combined\n"
           "with the <i>CtrlState->StateUnion.Text.Line</i>, overflows the\n"
           "remaining text control space.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidDataLength::getPrecondition() {
    return "Requires a text control that can be written to\n"
           "and which an overflow can occur, i.e. possible to\n"
           "write more to the last line in the control than\n"
           "can be stored.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidDataLength::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *
 * Try writing more to one line than can be written to.  Must write to the
 * last line to avoid overflowing to the next line.
 *****************************************************************************/

HpiTestStatus InvalidDataLength::runCtrlTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry,
                                             SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;

    if (!isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else if (!canOverflowLine(ctrlRec)) {
        // unable to overflow the text buffer
        status.assertNotSupport();
    } else {
        CharBuffer buffer(getDataType(ctrlRec), getLanguage(ctrlRec));
        buffer.fill(BYTE_VALUE_1, getMaxChars(ctrlRec) + 1);

        setDefaultCtrlState(ctrlRec, &ctrlState);
        ctrlState.StateUnion.Text.Line = getMaxLines(ctrlRec);
        ctrlState.StateUnion.Text.Text = buffer.getTextBuffer();

        SaErrorT error = saHpiControlSet(sessionId,
                                         rptEntry->ResourceId, ctrlRec->Num,
                                         SAHPI_CTRL_MODE_MANUAL, &ctrlState);

        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertNotSupport();
        } else {
            status.assertFailure(TRACE, CONTROL_SET, SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}

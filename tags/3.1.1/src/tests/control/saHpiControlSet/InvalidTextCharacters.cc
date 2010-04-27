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
 * Modified By:
 * 	Akmal Muqeeth
 * 	Pushpa Datla    
 */

#include "InvalidTextCharacters.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidTextCharacters::InvalidTextCharacters(char *line)
: ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getName() {
    return "InvalidTextCharacters";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getDescription() {
    return "The SaHpiTextBufferT structure passed as\n"
           "<i>CtrlState->StateUnion.Text.Text</i> must not contain text\n"
           "characters that are not allowed according to the value\n"
           "of <i>CtrlState->StateUnion.Text.Text.DataType</i>.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getPrecondition() {
    return "Requires a Text Control whose state can be set and whose "
           "DataType is SAHPI_TL_TYPE_UNICODE, SAHPI_TL_TYPE_ASCII6, "
           "or SAHPI_TL_TYPE_BCDPLUS.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidTextCharacters::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidTextCharacters::runCtrlTest(SaHpiSessionIdT sessionId,
                                                 SaHpiRptEntryT *rptEntry,
                                                 SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;

    if (!isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else if (!isAscii6DataType(ctrlRec) && 
               !isBcdPlusDataType(ctrlRec) && !isUnicodeDataType(ctrlRec)) {
        status.assertNotSupport();
    } else {

        setDefaultCtrlState(ctrlRec, &ctrlState);

        // Unicode requires an even number of bytes and ascii6 and bcdplus
        // don't support lowercase letters.

        ctrlState.StateUnion.Text.Text.DataLength = 1;
        ctrlState.StateUnion.Text.Text.Data[0] = 'a';

        SaErrorT error = saHpiControlSet(sessionId,
                                         rptEntry->ResourceId, ctrlRec->Num,
                                         SAHPI_CTRL_MODE_MANUAL, &ctrlState);

        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, CONTROL_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

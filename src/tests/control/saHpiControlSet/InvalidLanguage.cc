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

#include "InvalidLanguage.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidLanguage::InvalidLanguage(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidLanguage::getName() {
    return "InvalidLanguage";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidLanguage::getDescription() {
    return "The <i>DataType</i> specified in the RDR is\n"
           "SAHPI_TL_TYPE_UNICODE or SAHPI_TL_TYPE_TEXT and\n"
           "<i>CtrlState->StateUnion.Text.Text.Language</i> is\n"
           "not set to the <i>Language</i> specified in the RDR.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidLanguage::getPrecondition() {
    return "Requires a Text Control whose state can be set and "
           "whose DataType is SAHPI_TL_TYPE_UNICODE or SAHPI_TL_TYPE_TEXT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidLanguage::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidLanguage::runCtrlTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;

    if (!isTextControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else if (!isUnicodeDataType(ctrlRec) && !isTextDataType(ctrlRec)) {
        status.assertNotSupport();
    } else {

        setDefaultCtrlState(ctrlRec, &ctrlState);

        // Change the ctrl state to a different language.

        if (getLanguage(ctrlRec) == SAHPI_LANG_ZULU) {
            setLanguage(&ctrlState, SAHPI_LANG_CHINESE);
        } else {
            setLanguage(&ctrlState, SAHPI_LANG_ZULU);
        }

        SaErrorT error = saHpiControlSet(sessionId,
                                         rptEntry->ResourceId, ctrlRec->Num,
                                         SAHPI_CTRL_MODE_MANUAL, &ctrlState);

        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertNotSupport();
        } else {
            status.assertFailure(TRACE, CONTROL_SET,
                                 SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}

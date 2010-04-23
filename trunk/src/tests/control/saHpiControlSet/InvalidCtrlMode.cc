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
 * Modified By:
 *     Akmal Muqeeth
 *     Pushpa Datla
 */

#include "InvalidCtrlMode.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidCtrlMode::InvalidCtrlMode(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidCtrlMode::getName() {
    return "InvalidCtrlMode";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidCtrlMode::getDescription() {
    return "<i>CtrlMode</i> is not one of the valid enumerated\n"
           "values for this type.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidCtrlMode::getPrecondition() {
    return "Requires a Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidCtrlMode::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidCtrlMode::runCtrlTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry,
                                           SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;
    int invalidCtrlMode = (int) SAHPI_CTRL_MODE_MANUAL + 1;

    if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {
        setDefaultCtrlState(ctrlRec, &ctrlState);
        SaErrorT error = saHpiControlSet(sessionId,
                              rptEntry->ResourceId, ctrlRec->Num,
                              (SaHpiCtrlModeT) invalidCtrlMode, &ctrlState);

        if ((error == SA_ERR_HPI_INVALID_DATA) ||
            (error == SA_ERR_HPI_INVALID_REQUEST)) {
            status.assertNotSupport();

        } else if (error == SA_ERR_HPI_INVALID_PARAMS)
 {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, CONTROL_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

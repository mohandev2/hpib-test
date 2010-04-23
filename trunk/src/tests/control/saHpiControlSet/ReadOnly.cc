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

#include "ReadOnly.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnly::ReadOnly(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnly::getName() {
    return "ReadOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnly::getDescription() {
    return "Attempt to change the mode of a control with a read-only mode.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires a Control whose mode cannot be changed.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnly::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReadOnly::runCtrlTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;

    if (!isReadOnlyMode(ctrlRec)) {
        status.assertNotSupport();
    } else {

        if (ctrlRec->DefaultMode.Mode == SAHPI_CTRL_MODE_AUTO) {
            ctrlMode = SAHPI_CTRL_MODE_MANUAL;
        } else {
            ctrlMode = SAHPI_CTRL_MODE_AUTO;
        }

        setDefaultCtrlState(ctrlRec, &ctrlState);

        SaErrorT error = saHpiControlSet(sessionId, rptEntry->ResourceId, 
                                         ctrlRec->Num, ctrlMode, &ctrlState);

        if ((error == SA_ERR_HPI_INVALID_DATA) ||
            (error == SA_ERR_HPI_INVALID_REQUEST)) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, CONTROL_SET,
                                 SA_ERR_HPI_READ_ONLY, error);
        }
    }

    return status;
}

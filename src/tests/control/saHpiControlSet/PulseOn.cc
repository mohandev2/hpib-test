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

#include "PulseOn.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

PulseOn::PulseOn(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *PulseOn::getName() {
    return "PulseOn";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *PulseOn::getDescription() {
    return "Send SAHPI_CTRL_STATE_PULSE_ON to a digital control which is ON.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *PulseOn::getPrecondition() {
    return "Requires a readable Digital Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT PulseOn::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus PulseOn::runCtrlTest(SaHpiSessionIdT sessionId,
                                   SaHpiRptEntryT *rptEntry,
                                   SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;

    if (!isDigitalControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {

        SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &ctrlMode, &ctrlState);

        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else if (ctrlState.StateUnion.Digital == SAHPI_CTRL_STATE_ON) {
            setDefaultCtrlState(ctrlRec, &ctrlState);
            ctrlState.StateUnion.Digital = SAHPI_CTRL_STATE_PULSE_ON;

            error = saHpiControlSet(sessionId, rptEntry->ResourceId, ctrlRec->Num,
                                    SAHPI_CTRL_MODE_MANUAL, &ctrlState);

            if (error == SA_ERR_HPI_INVALID_DATA) {
                status.assertNotSupport();
            } else if (error == SA_ERR_HPI_INVALID_REQUEST) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, CONTROL_SET,
                                     SA_ERR_HPI_INVALID_REQUEST, error);
            }
        }
    }

    return status;
}

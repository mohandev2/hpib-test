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

#include "SetDigital.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetDigital::SetDigital(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetDigital::getName() {
    return "SetDigital";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetDigital::getDescription() {
    return "Set the value of a digital control to\n"
           "SAHPI_CTRL_STATE_ON and SAHPI_CTRL_STATE_OFF.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetDigital::getPrecondition() {
    return "Requires a readable Digital Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetDigital::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Test setting the Digital Control to ON and OFF.
 *****************************************************************************/

HpiTestStatus SetDigital::runCtrlTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (ctrlRec->WriteOnly || 
            !isDigitalControl(ctrlRec) || !canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                         ctrlNum, &ctrlMode, &ctrlState);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else {
            status.add(TRACE, setDigitalValue(sessionId, resourceId,
                                              ctrlRec, SAHPI_CTRL_STATE_OFF));

            status.add(TRACE, setDigitalValue(sessionId, resourceId,
                                              ctrlRec, SAHPI_CTRL_STATE_ON));

            // Restore original mode and state

            error = saHpiControlSet(sessionId, resourceId,
                                    ctrlNum, ctrlMode, &ctrlState);
            if ((error != SA_OK) &&
                (error != SA_ERR_HPI_INVALID_DATA) &&
                (error != SA_ERR_HPI_INVALID_REQUEST)) {
                status.assertError(TRACE, CONTROL_SET, SA_OK, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Set the value of a digital control.
 *****************************************************************************/

HpiTestStatus SetDigital::setDigitalValue(SaHpiSessionIdT sessionId,
                                          SaHpiResourceIdT resourceId,
                                          SaHpiCtrlRecT * ctrlRec,
                                          SaHpiCtrlStateDigitalT value) {
    HpiTestStatus status;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiCtrlStateT newCtrlState;

    setDefaultCtrlState(ctrlRec, &newCtrlState);
    newCtrlState.StateUnion.Digital = value;
    SaErrorT error = saHpiControlSet(sessionId, resourceId, ctrlNum,
                                     SAHPI_CTRL_MODE_MANUAL, &newCtrlState);
    if ((error == SA_ERR_HPI_INVALID_DATA) ||
        (error == SA_ERR_HPI_INVALID_REQUEST)) {
        status.assertNotSupport();
    } else if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, CONTROL_SET, SA_OK, error);
    }

    return status;
}


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

#include "SetAnalog.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetAnalog::SetAnalog(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetAnalog::getName() {
    return "SetAnalog";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetAnalog::getDescription() {
    return "Set an analog control's value to it's minimum and maximum values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetAnalog::getPrecondition() {
    return "Requires a readable Analog Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetAnalog::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * Test setting the analog value to the minimum and maximum values.
 *****************************************************************************/

HpiTestStatus SetAnalog::runCtrlTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (!(!ctrlRec->WriteOnly && 
                isAnalogControl(ctrlRec) && canSetControlState(ctrlRec))) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                         ctrlNum, &ctrlMode, &ctrlState);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else {
            status.add(TRACE, setAnalogValue(sessionId,
                                             resourceId,
                                             ctrlRec,
                                             ctrlRec->TypeUnion.Analog.Min));

            status.add(TRACE, setAnalogValue(sessionId,
                                             resourceId,
                                             ctrlRec,
                                             ctrlRec->TypeUnion.Analog.Max));

            // Restore original mode and state

            error = saHpiControlSet(sessionId, resourceId, ctrlNum, ctrlMode, &ctrlState);
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
 * Set the Analog value.
 *****************************************************************************/

HpiTestStatus SetAnalog::setAnalogValue(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiCtrlRecT * ctrlRec,
                                        SaHpiCtrlStateAnalogT value) {
    HpiTestStatus status;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiCtrlStateT newCtrlState;

    setDefaultCtrlState(ctrlRec, &newCtrlState);
    newCtrlState.StateUnion.Analog = value;
    SaErrorT error = saHpiControlSet(sessionId, resourceId,
                                     ctrlNum, SAHPI_CTRL_MODE_MANUAL, &newCtrlState);
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



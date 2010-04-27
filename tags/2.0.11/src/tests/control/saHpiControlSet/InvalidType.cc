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

#include "InvalidType.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidType::InvalidType(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidType::getName() {
    return "InvalidType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidType::getDescription() {
    return "<i>CtrlState->Type</i> field is not the correct type for the control.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidType::getPrecondition() {
    return "Requires a Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidType::runCtrlTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;

    if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {
        // Set up the default ctrlState and then change
        // it's Type to something else.
         
        if (ctrlRec->Type == SAHPI_CTRL_TYPE_DISCRETE) {
            ctrlState.Type = SAHPI_CTRL_TYPE_DIGITAL;
            ctrlState.StateUnion.Digital = SAHPI_CTRL_STATE_OFF;
        } else {
            ctrlState.Type = SAHPI_CTRL_TYPE_DISCRETE;
            ctrlState.StateUnion.Discrete = 0;
        }
         
        SaErrorT error = saHpiControlSet(sessionId,
                                         rptEntry->ResourceId, ctrlRec->Num,
                                         SAHPI_CTRL_MODE_MANUAL, &ctrlState);
         
        if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertNotSupport();
        } else if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, CONTROL_SET, SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}

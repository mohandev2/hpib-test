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

#include "InvalidAnalog.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAnalog::InvalidAnalog(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAnalog::getName() {
    return "InvalidAnalog";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAnalog::getDescription() {
    return "<i>CtrlState->StateUnion.Analog</i> is out of range of the\n"
           "control record's analog Min and Max values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidAnalog::getPrecondition() {
    return "Requires an Analog control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAnalog::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *
 * Try setting values above and below the Max and Min, respectively.
 *****************************************************************************/

HpiTestStatus InvalidAnalog::runCtrlTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry,
                                         SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlStateT ctrlState;

    if (!isAnalogControl(ctrlRec)) {
        status.assertNotSupport();
    } else if (!canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {
        setDefaultCtrlState(ctrlRec, &ctrlState);

        ctrlState.StateUnion.Analog = ctrlRec->TypeUnion.Analog.Max + 1;
        SaErrorT error = saHpiControlSet(sessionId,
                                         rptEntry->ResourceId, ctrlRec->Num,
                                         SAHPI_CTRL_MODE_MANUAL, &ctrlState);

        if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertNotSupport();
        } else if (error != SA_ERR_HPI_INVALID_DATA) {
            status.assertFailure(TRACE, CONTROL_SET, SA_ERR_HPI_INVALID_DATA, error,
                                 "Setting a value above the maximum.");
        } 

        ctrlState.StateUnion.Analog = ctrlRec->TypeUnion.Analog.Min - 1;
        error = saHpiControlSet(sessionId, rptEntry->ResourceId, ctrlRec->Num,
                                SAHPI_CTRL_MODE_MANUAL, &ctrlState);

        if (error == SA_ERR_HPI_INVALID_REQUEST) {
            status.assertNotSupport();
        } else if (error != SA_ERR_HPI_INVALID_DATA) {
            status.assertFailure(TRACE, CONTROL_SET, SA_ERR_HPI_INVALID_DATA, error,
                                 "Setting a value below the minimum.");
        } 

        if (status.isOkay()) {
            status.assertPass();
        }
    }

    return status;
}

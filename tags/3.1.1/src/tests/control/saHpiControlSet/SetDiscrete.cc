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

#include "SetDiscrete.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetDiscrete::SetDiscrete(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetDiscrete::getName() {
    return "SetDiscrete";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetDiscrete::getDescription() {
    return "Set a discrete control's value to its default value.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetDiscrete::getPrecondition() {
    return "Requires a readable Discrete Control whose state can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetDiscrete::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetDiscrete::runCtrlTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (ctrlRec->WriteOnly || 
            !isDiscreteControl(ctrlRec) || !canSetControlState(ctrlRec)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiControlGet(sessionId, resourceId,
                                         ctrlNum, &ctrlMode, &ctrlState);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else {
	  status.add(TRACE, setDiscreteValue(sessionId,
					     resourceId,
					     ctrlRec,
					     ctrlRec->TypeUnion.Discrete.Default));

	  // Restore original mode and state
	  error = saHpiControlSet(sessionId, resourceId,
				  ctrlNum, ctrlMode, &ctrlState);
	  if ((error != SA_OK) &&
	      (error != SA_ERR_HPI_INVALID_DATA) &&
	      (error != SA_ERR_HPI_INVALID_REQUEST) &&
	      (error != SA_ERR_HPI_UNSUPPORTED_PARAMS)) {
	    status.assertError(TRACE, CONTROL_SET, SA_OK, error);
	  }
       }
    }

    return status;
}

/*****************************************************************************
 * Set a discrete control's value.
 *****************************************************************************/

HpiTestStatus SetDiscrete::setDiscreteValue(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId,
                                            SaHpiCtrlRecT * ctrlRec,
                                            SaHpiCtrlStateDiscreteT value) {
    HpiTestStatus status;
    SaHpiCtrlNumT ctrlNum = ctrlRec->Num;
    SaHpiCtrlStateT newCtrlState;

    setDefaultCtrlState(ctrlRec, &newCtrlState);
    newCtrlState.StateUnion.Discrete = value;
    SaErrorT error = saHpiControlSet(sessionId, resourceId, ctrlNum,
                                     SAHPI_CTRL_MODE_MANUAL, &newCtrlState);
    if ((error == SA_ERR_HPI_INVALID_DATA) ||
        (error == SA_ERR_HPI_INVALID_REQUEST) ||
	(error != SA_ERR_HPI_UNSUPPORTED_PARAMS)) {
        status.assertNotSupport();
    } else if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, CONTROL_SET, SA_OK, error);
    }

    return status;
}


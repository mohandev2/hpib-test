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

#include "Mid.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Mid::Mid(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Mid::getName() {
    return "MId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Mid::getDescription() {
    return "The ManufacturerId (MId) field of an OEM control is ignored "
           "by the implementation when calling saHpiControlSet().";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Mid::getPrecondition() {
    return "Requires a readable OEM Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Mid::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *
 * For an OEM control that we can write to, read the OEM's MId field
 * and then try to change it.  Retrieve it again to verify that the set
 * failed as it should.
 *****************************************************************************/

HpiTestStatus Mid::runCtrlTest(SaHpiSessionIdT sessionId,
                               SaHpiRptEntryT *rptEntry,
                               SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT ctrlMode;
    SaHpiCtrlStateT ctrlState;
    SaHpiCtrlModeT origCtrlMode;
    SaHpiCtrlStateT origCtrlState;
    SaHpiCtrlStateT newCtrlState;

    if (ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else if (!isOemControl(ctrlRec)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiControlGet(sessionId,
                                         rptEntry->ResourceId, ctrlRec->Num,
                                         &origCtrlMode, &origCtrlState);
        if (error != SA_OK) {
            status.assertError(TRACE, CONTROL_GET, SA_OK, error);
        } else if (origCtrlMode == SAHPI_CTRL_MODE_AUTO && ctrlRec->DefaultMode.ReadOnly) {
            status.assertNotSupport();
        } else {
            newCtrlState = origCtrlState;
            newCtrlState.StateUnion.Oem.MId++;
            error = saHpiControlSet(sessionId, rptEntry->ResourceId,
                                    ctrlRec->Num, SAHPI_CTRL_MODE_MANUAL, &newCtrlState);
            if ((error == SA_ERR_HPI_INVALID_DATA) ||
                (error == SA_ERR_HPI_INVALID_REQUEST)) {
                status.assertNotSupport();
            } else if (error != SA_OK) {
                status.assertError(TRACE, CONTROL_SET, SA_OK, error);
            } else {
                error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                        ctrlRec->Num, &ctrlMode, &ctrlState);
                if (error != SA_OK) {
                    status.assertError(TRACE, CONTROL_GET, SA_OK, error);
                } else if (ctrlState.StateUnion.Oem.MId == newCtrlState.StateUnion.Oem.MId) {
                    status.assertFailure(TRACE, 
                        "ManufacturerId field of an OEM control was set.");
                } else {
                    status.assertPass();
                }

                error = saHpiControlSet(sessionId, rptEntry->ResourceId,
                                        ctrlRec->Num, origCtrlMode, &origCtrlState);
                if ((error != SA_OK) &&
                    (error != SA_ERR_HPI_INVALID_DATA) &&
                    (error != SA_ERR_HPI_INVALID_REQUEST)) {
                    status.assertError(TRACE, CONTROL_SET, SA_OK, error);
                }
            }
        }
    }

    return status;
}

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

#include "WriteOnly.h"
#include "ControlHelper.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

WriteOnly::WriteOnly(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *WriteOnly::getName() {
    return "WriteOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *WriteOnly::getDescription() {
    return "Get the <i>CtrlMode</i> and <i>CtrlState</i> for a\n"
           "write-only control.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *WriteOnly::getPrecondition() {
    return "Requires a write-only Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT WriteOnly::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *
 * Try various combinations of pointers to make sure nothing weird is
 * going on in the implementation.
 *****************************************************************************/

HpiTestStatus WriteOnly::runCtrlTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry,
                                     SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlModeT mode;
    SaHpiCtrlStateT state;

    if (!ctrlRec->WriteOnly) {
        status.assertNotSupport();
    } else {
        status.assertPass();

        SaErrorT error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &mode, &state);
        if (error != SA_ERR_HPI_INVALID_CMD) {
            status.assertFailure(TRACE, CONTROL_GET,
                                 SA_ERR_HPI_INVALID_CMD, error);
        } 

        error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                ctrlRec->Num, NULL, &state);
        if (error != SA_ERR_HPI_INVALID_CMD) {
            status.assertFailure(TRACE, CONTROL_GET,
                                 SA_ERR_HPI_INVALID_CMD, error,
                                 "Using NULL CtrlMode pointer.");
        } 

        error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                ctrlRec->Num, &mode, NULL);
        if (error != SA_ERR_HPI_INVALID_CMD) {
            status.assertFailure(TRACE, CONTROL_GET,
                                 SA_ERR_HPI_INVALID_CMD, error,
                                 "Using NULL CtrlState pointer.");
        } 

        error = saHpiControlGet(sessionId, rptEntry->ResourceId,
                                ctrlRec->Num, NULL, NULL);
        if (error != SA_ERR_HPI_INVALID_CMD) {
            status.assertFailure(TRACE, CONTROL_GET,
                                 SA_ERR_HPI_INVALID_CMD, error,
                                 "Using NULL CtrlMode and NULL CtrlState pointers.");
        } 
    }

    return status;
}

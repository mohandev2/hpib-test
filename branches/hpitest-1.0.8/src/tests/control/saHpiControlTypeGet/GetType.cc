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

#include "GetType.h"
#include "ControlHelper.h"

using namespace ns_saHpiControlTypeGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetType::GetType(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetType::getName() {
    return "GetType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetType::getDescription() {
    return "Get the control type and verify that it is correct.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetType::getPrecondition() {
    return "Requires a Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetType::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetType::runCtrlTest(SaHpiSessionIdT sessionId,
                                   SaHpiRptEntryT *rptEntry,
                                   SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;
    SaHpiCtrlTypeT type;

    SaErrorT error = saHpiControlTypeGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, &type);
    if (error != SA_OK) {
        status.assertFailure(TRACE, CONTROL_TYPE_GET, SA_OK, error);
    } else if (!ControlHelper::isValidCtrlType(type)) {
        status.assertFailure(TRACE, "Control Type is invalid: 0x%X", type);
    } else {
        status.assertPass();
    }

    return status;
}

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

#include "InvalidAction.h"

using namespace ns_saHpiParmControl;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidAction::InvalidAction(char *line) : ParmControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidAction::getName() {
    return "InvalidAction";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidAction::getDescription() {
    return "The <i>Action</i> parameter is not a proper value.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidAction::getPrecondition() {
    return "Requires a resource that Configuration.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidAction::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidAction::runParmTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    int invalidAction = (int) SAHPI_RESTORE_PARM + 1;

    SaErrorT error = saHpiParmControl(sessionId, rptEntry->ResourceId, 
                                      (SaHpiParmActionT) invalidAction);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, PARM_CONTROL,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

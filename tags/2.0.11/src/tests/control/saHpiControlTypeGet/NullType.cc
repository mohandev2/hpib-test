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

#include "NullType.h"

using namespace ns_saHpiControlTypeGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullType::NullType(char *line) : ControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullType::getName() {
    return "NullType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullType::getDescription() {
    return "The <i>Type</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullType::getPrecondition() {
    return "Requires a Control.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullType::runCtrlTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiCtrlRecT *ctrlRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiControlTypeGet(sessionId, rptEntry->ResourceId,
                                         ctrlRec->Num, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, CONTROL_TYPE_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}

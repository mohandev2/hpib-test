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

#include "InvalidState.h"

using namespace ns_saHpiResourcePowerStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidState::InvalidState(char *line) : PowerTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidState::getName() {
    return "InvalidState";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidState::getDescription() {
    return "The <i>State</i> parameter is not one of the\n"
           "valid enumerated values.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidState::getPrecondition() {
    return "Requires a resource that supports the POWER capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidState::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidState::runPowerTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    int invalidState = (int) SAHPI_POWER_CYCLE + 1;

    SaErrorT error = saHpiResourcePowerStateSet(sessionId, rptEntry->ResourceId, 
                                                (SaHpiPowerStateT) invalidState);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RESOURCE_POWER_STATE_SET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

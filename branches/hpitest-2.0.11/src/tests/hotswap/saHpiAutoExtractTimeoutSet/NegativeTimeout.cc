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

#include "NegativeTimeout.h"

using namespace ns_saHpiAutoExtractTimeoutSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NegativeTimeout::NegativeTimeout(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NegativeTimeout::getName() {
    return "NegativeTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NegativeTimeout::getDescription() {
    return "The <i>Timeout</i> parameter is passed in with a negative\n"
           "value, i.e. it is not SAHPI_TIMEOUT_BLOCK, SAHPI_TIMEOUT_IMMEDIATE,\n"
           "or a positive value.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NegativeTimeout::getPrecondition() {
    return "Requires a managed hot swap resource whose Auto Extract Timeout can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NegativeTimeout::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NegativeTimeout::runHsTest(SaHpiSessionIdT sessionId,
                                         SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (hsAutoExtractReadOnly) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAutoExtractTimeoutSet(sessionId,
                                                    rptEntry->ResourceId, -2);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, AUTO_EXTRACT_TIMEOUT_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

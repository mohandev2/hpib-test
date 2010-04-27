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

#include "NullTimeout.h"

using namespace ns_saHpiAutoExtractTimeoutGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullTimeout::NullTimeout(char *line) : HotSwapTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullTimeout::getName() {
    return "NullTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullTimeout::getDescription() {
    return "The <i>Timeout</i> parameter is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullTimeout::getPrecondition() {
    return "A managed hot swap resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullTimeout::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Runt the test.
 *****************************************************************************/

HpiTestStatus NullTimeout::runHsTest(SaHpiSessionIdT sessionId,
                                     SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    SaErrorT error = saHpiAutoExtractTimeoutGet(sessionId,
                                                rptEntry->ResourceId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, AUTO_EXTRACT_TIMEOUT_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

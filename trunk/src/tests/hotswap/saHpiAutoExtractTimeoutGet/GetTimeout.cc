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

#include "GetTimeout.h"

using namespace ns_saHpiAutoExtractTimeoutGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetTimeout::GetTimeout(char *line) : HotSwapTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetTimeout::getName() {
    return "GetTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetTimeout::getDescription() {
    return "Retrieve the timeout for how long a resource will wait\n"
           "before the default auto-extraction policy is invoked.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetTimeout::getPrecondition() {
    return "A managed hot swap resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetTimeout::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetTimeout::runHsTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiTimeoutT timeout;

    SaErrorT error = saHpiAutoExtractTimeoutGet(sessionId,
                                                rptEntry->ResourceId,
                                                &timeout);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, AUTO_EXTRACT_TIMEOUT_GET, SA_OK, error);
    }

    return status;
}

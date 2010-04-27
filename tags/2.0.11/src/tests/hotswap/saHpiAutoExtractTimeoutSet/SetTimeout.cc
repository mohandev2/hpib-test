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

#include "SetTimeout.h"

using namespace ns_saHpiAutoExtractTimeoutSet;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define ONE_SECOND 1000000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetTimeout::SetTimeout(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetTimeout::getName() {
    return "SetTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetTimeout::getDescription() {
    return "Set the timeout value for how long to wait before the default\n"
           "auto-extraction policy is invoked.  Several values are tested:\n"
           "one second, SAHPI_TIMEOUT_IMMEDIATE, and SAHPI_TIMEOUT_BLOCK.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *SetTimeout::getPrecondition() {
    return "Requires a managed hot swap resource whose\n"
           "Auto Extract Timeout can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetTimeout::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetTimeout::runHsTest(SaHpiSessionIdT sessionId,
                                    SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiTimeoutT newTimeout;
    SaHpiTimeoutT timeout[] = { ONE_SECOND,
                                SAHPI_TIMEOUT_IMMEDIATE,
                                SAHPI_TIMEOUT_BLOCK };

    if (hsAutoExtractReadOnly) {
        status.assertNotSupport();
    } else {
        for (int i = 0; i < 3; i++) {
            SaErrorT error = saHpiAutoExtractTimeoutSet(sessionId,
                                                        rptEntry->ResourceId,
                                                        timeout[i]);
            if (error != SA_OK) {
                status.assertFailure(TRACE, AUTO_EXTRACT_TIMEOUT_SET,
                                     SA_OK, error);
            } else {
                error = saHpiAutoExtractTimeoutGet(sessionId,
                                                   rptEntry->ResourceId,
                                                   &newTimeout);
                if (error != SA_OK) {
                    status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_GET,
                                       SA_OK, error);
                } else if (newTimeout == timeout[i]) {
                    status.assertPass();
                } else {
                    status.assertFailure(TRACE, "Set failed. "
                            "Retrieved timeout of %s did not "
                            "match the set timeout of %s.",
                            HpiString::timeout(newTimeout),
                            HpiString::timeout(timeout[i]));
                }
            }
        }

        status.add(TRACE, restoreTimeouts(sessionId, rptEntry->ResourceId));
    }

    return status;
}

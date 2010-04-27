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

#include "AutoExtractTimeout.h"

using namespace ns_saHpiParmControl;

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define ONE_SECOND  1000000000
#define TWO_SECONDS 2000000000

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AutoExtractTimeout::AutoExtractTimeout(char *line) : ParmControlTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AutoExtractTimeout::getName() {
    return "AutoExtractTimeout";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AutoExtractTimeout::getDescription() {
    return "For a managed hot swap resource, do the following:\n"
           "<ol>\n"
           "<li>Save the resource's configuraton parameters.</li>\n"
           "<li>Change the resource's auto-extract timeout value.</li>\n"
           "<li>Restore the resource's configuration parameters.</li>\n"
           "<li>Verify that the resource's auto-extract timeout value\n"
               "was restored.</li>\n"
           "</ol>";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AutoExtractTimeout::getPrecondition() {
    return "Requires a managed hot swap resource whose Auto Extract Timeout can be set.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AutoExtractTimeout::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AutoExtractTimeout::runParmTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiTimeoutT origTimeout;
    SaHpiTimeoutT newTimeout;

    if (!hasManagedHotSwapCapability(rptEntry)) {
        status.assertNotSupport();
    } else if (rptEntry->HotSwapCapabilities & SAHPI_HS_CAPABILITY_AUTOEXTRACT_READ_ONLY) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAutoExtractTimeoutGet(sessionId, resourceId, &origTimeout);
        if (error != SA_OK) {
            status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_GET, SA_OK, error);
        } else {
            error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_SAVE_PARM);
            if (error != SA_OK) {
                status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Saving parameters.");
            } else {
                SaHpiTimeoutT timeout = (origTimeout == ONE_SECOND) ? TWO_SECONDS : ONE_SECOND;
                error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, timeout);
                if (error != SA_OK) {
                    status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, SA_OK, error);
                } else {
                    error = saHpiParmControl(sessionId, rptEntry->ResourceId, SAHPI_RESTORE_PARM);
                    if (error != SA_OK) {
                        status.assertFailure(TRACE, PARM_CONTROL, SA_OK, error, "Restoring parameters.");
                    } else {
                        error = saHpiAutoExtractTimeoutGet(sessionId, resourceId, &newTimeout);
                        if (error != SA_OK) {
                            status.assertError(TRACE, AUTO_EXTRACT_TIMEOUT_GET, SA_OK, error);
                        } else if (origTimeout != newTimeout) {
                            status.assertFailure(TRACE, "AutoExtractTimeout was not restored!");
                        } else {
                            status.assertPass();
                        }
                    }
                    error = saHpiAutoExtractTimeoutSet(sessionId, resourceId, origTimeout);
                    status.checkError(TRACE, AUTO_EXTRACT_TIMEOUT_SET, error);
                }
            }
        }
    }

    return status;
}

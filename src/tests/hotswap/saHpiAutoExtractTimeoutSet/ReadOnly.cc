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

#include "ReadOnly.h"

using namespace ns_saHpiAutoExtractTimeoutSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ReadOnly::ReadOnly(char *line) : HotSwapTestCase(line, true) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ReadOnly::getName() {
    return "ReadOnly";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ReadOnly::getDescription() {
    return "The auto-extract timeout for the resource is fixed, as\n"
           "indicated by the SAHPI_HS_CAPABILITY_AUTOEXTRACT_READ_ONLY\n"
           "in the resource's RPT entry.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ReadOnly::getPrecondition() {
    return "Requires a managed hot swap resource whose\n"
           "Auto Extract Timeout is read only.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ReadOnly::getExpectedReturn() {
    return SA_ERR_HPI_READ_ONLY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ReadOnly::runHsTest(SaHpiSessionIdT sessionId,
                                  SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hsAutoExtractReadOnly) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAutoExtractTimeoutSet(sessionId,
                                                    rptEntry->ResourceId,
                                                    SAHPI_TIMEOUT_IMMEDIATE);
        if (error == SA_ERR_HPI_READ_ONLY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, AUTO_EXTRACT_TIMEOUT_SET,
                                 SA_ERR_HPI_READ_ONLY, error);
        }
    }

    return status;
}

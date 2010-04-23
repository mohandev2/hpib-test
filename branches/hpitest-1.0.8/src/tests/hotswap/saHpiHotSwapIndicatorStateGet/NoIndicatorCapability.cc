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

#include "NoIndicatorCapability.h"

using namespace ns_saHpiHotSwapIndicatorStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoIndicatorCapability::NoIndicatorCapability(char *line)
: HotSwapTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoIndicatorCapability::getName() {
    return "NoIndicatorCapability";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoIndicatorCapability::getDescription() {
    return "The resource does not support the hot swap indicator on the FRU\n"
           "as indicated by the SAHPI_HS_CAPABILITY_INDICATOR_SUPPORTED in\n"
           "the resource's RPT entry.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoIndicatorCapability::getPrecondition() {
    return "Requires a managed hot swap resource that does not support a hot swap indicator.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoIndicatorCapability::getExpectedReturn() {
    return SA_ERR_HPI_CAPABILITY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoIndicatorCapability::runHsTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiHsIndicatorStateT state;

    if (isIndicatorSupported(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiHotSwapIndicatorStateGet(sessionId,
                                                       rptEntry->ResourceId,
                                                       &state);
        if (error == SA_ERR_HPI_CAPABILITY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, HOT_SWAP_INDICATOR_STATE_GET,
                                 SA_ERR_HPI_CAPABILITY, error);
        }
    }

    return status;
}

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

#include "NoHotSwapCapability.h"

using namespace ns_saHpiHotSwapIndicatorStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoHotSwapCapability::NoHotSwapCapability(char *line)
: HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoHotSwapCapability::getName() {
    return "NoHotSwapCapability";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoHotSwapCapability::getDescription() {
    return "The resource does not support managed hot swap, as indicated\n"
           "by SAHPI_CAPABILITY_MANAGED_HOTSWAP in the resource's RPT entry.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoHotSwapCapability::getPrecondition() {
    return "Requires a resource that does not support managed hot swap.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoHotSwapCapability::getExpectedReturn() {
    return SA_ERR_HPI_CAPABILITY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoHotSwapCapability::runResourceTest(SaHpiSessionIdT sessionId,
                                                   SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (hasManagedHotSwapCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiHotSwapIndicatorStateSet(sessionId,
                                                       rptEntry->ResourceId,
                                                       SAHPI_HS_INDICATOR_OFF);
        if (error == SA_ERR_HPI_CAPABILITY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, HOT_SWAP_INDICATOR_STATE_SET,
                                 SA_ERR_HPI_CAPABILITY, error);
        }
    }

    return status;
}

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

#include "NullIdrInfo.h"

using namespace ns_saHpiIdrInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullIdrInfo::NullIdrInfo(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullIdrInfo::getName() {
    return "NullIdrInfo";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullIdrInfo::getDescription() {
    return "The <i>IdrInfo</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullIdrInfo::getPrecondition() {
    return "Requires an Inventory.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullIdrInfo::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullIdrInfo::runIdrTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiIdrInfoGet(sessionId, rptEntry->ResourceId,
                                     idrRec->IdrId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_INFO_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


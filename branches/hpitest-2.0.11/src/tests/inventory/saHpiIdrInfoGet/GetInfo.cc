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

#include "GetInfo.h"

using namespace ns_saHpiIdrInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetInfo::GetInfo(char *line) : IdrTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetInfo::getName() {
    return "GetInfo";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetInfo::getDescription() {
    return "Get the IDR info.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetInfo::getPrecondition() {
    return "Requires an Inventory.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetInfo::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetInfo::runIdrTest(SaHpiSessionIdT sessionId,
                                  SaHpiRptEntryT *rptEntry,
                                  SaHpiInventoryRecT *idrRec) {
    HpiTestStatus status;
    SaHpiIdrInfoT info;

    SaErrorT error = saHpiIdrInfoGet(sessionId, rptEntry->ResourceId,
                                     idrRec->IdrId, &info);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, IDR_INFO_GET, SA_OK, error);
    } 

    return status;
}


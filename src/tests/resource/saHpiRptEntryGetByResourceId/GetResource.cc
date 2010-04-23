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

#include "GetResource.h"
#include "Report.h"
#include "ResourceHelper.h"

using namespace ns_saHpiRptEntryGetByResourceId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetResource::GetResource(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetResource::getName() {
    return "GetResource";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetResource::getDescription() {
    return "Retrieve the resource information for an existing resource.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetResource::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetResource::runResourceTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    Report report;
    SaHpiRptEntryT curRptEntry;

    SaErrorT error = saHpiRptEntryGetByResourceId(sessionId, 
                                                  rptEntry->ResourceId,
                                                  &curRptEntry);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
    } else if (!ResourceHelper::isEqual(rptEntry, &curRptEntry, report)) {
        status.assertFailure(TRACE, report.toString());
    } else {
        status.assertPass();
    }

    return status;
}


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

#include "GetAll.h"
#include "Report.h"
#include "RdrHelper.h"

using namespace ns_saHpiRdrGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetAll::GetAll(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetAll::getName() {
    return "GetAll";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetAll::getDescription() {
    return "Get all of the resource data records for a resource.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *GetAll::getPrecondition() {
    return "Requires one or more resources with the RDR capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetAll::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetAll::runResourceTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasRdrCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        status.assertPass();
        Report report;
        SaHpiRdrT rdr;
        SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
        while (nextEntryId != SAHPI_LAST_ENTRY) {
            SaHpiEntryIdT entryId = nextEntryId;
            SaErrorT error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                         entryId, &nextEntryId, &rdr);
            if (error != SA_OK) {
                status.assertFailure(TRACE, RDR_GET, SA_OK, error);
                break;
            } else if (!RdrHelper::isValid(&rdr, report)) {
                status.assertFailure(TRACE, report.toString());
                break;
            }
        }
    }

    return status;
}


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

#include "GetResourceId.h"

using namespace ns_saHpiResourceIdGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetResourceId::GetResourceId(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetResourceId::getName() {
    return "GetResourceId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetResourceId::getDescription() {
    return "Retrieve the ResourceId of the resource associated with\n"
           "the entity upon which the HPI User is running.";
}

/*****************************************************************************
 * Return the precondition.
 *****************************************************************************/

const char *GetResourceId::getPrecondition() {
    return "The entity the HPI User is running on must be manageable and the\n"
           "domain controller must be able to determine an appropriate response.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetResourceId::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetResourceId::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId;
    SaHpiRptEntryT rptEntry;

    SaErrorT error = saHpiResourceIdGet(sessionId, &resourceId);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_UNKNOWN) { 
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_ID_GET, SA_OK, error);
    } else {
        error = saHpiRptEntryGetByResourceId(sessionId, resourceId, &rptEntry);
        if (error == SA_OK) {
            status.assertPass();
        } else if (error == SA_ERR_HPI_INVALID_RESOURCE) {
            status.assertFailure(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
        } else {
            status.assertError(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
        }
    }

    return status;
}


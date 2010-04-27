/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 */

#include "GetEntityPath.h"

using namespace ns_saHpiMyEntityPathGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetEntityPath::GetEntityPath(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetEntityPath::getName() {
    return "GetEntityPath";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetEntityPath::getDescription() {
    return "Retrieve the EntityPath of the resource associated with\n"
           "the entity upon which the HPI User is running.";
}

/*****************************************************************************
 * Return the precondition.
 *****************************************************************************/

const char *GetEntityPath::getPrecondition() {
    return "The entity the HPI User is running on must be manageable and the\n"
           "domain controller must be able to determine an appropriate response.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetEntityPath::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetEntityPath::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus 		status;
    SaHpiEntityPathT 	entitypath;
    SaHpiUint32T 		instanceId = SAHPI_FIRST_ENTRY;
    SaHpiResourceIdT 	resourceId;
    SaHpiInstrumentIdT 	instrumentId; 
    SaHpiUint32T 		rptUpdateCount;

    SaErrorT error = saHpiMyEntityPathGet(sessionId, &entitypath);
    if (error == SA_ERR_HPI_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_UNKNOWN) { 
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, MY_ENTITY_PATH_GET, SA_OK, error);
    } else {
        error = saHpiGetIdByEntityPath(sessionId, entitypath, SAHPI_NO_RECORD,
        	                               &instanceId, &resourceId, &instrumentId,
        	                               &rptUpdateCount);
        if (error == SA_OK) {
            status.assertPass();
        } else if (error == SA_ERR_HPI_INVALID_RESOURCE) {
            status.assertFailure(TRACE, GET_ID_BY_ENTITY_PATH, SA_OK, error);
        } else {
            status.assertError(TRACE, GET_ID_BY_ENTITY_PATH, SA_OK, error);
        }
    }

    return status;
}


/*      
 * (C) Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Akmal A Muqeeth <aax6@unh.edu>
 *
 * 09/08/11 lars.wetzel@emerson.com
 *          Fix Mantis 248: rc INVALID_CMD and INVALID_REQUEST are allowed
 */

#include "NoFruCapability.h"

using namespace ns_saHpiResourceFailedRemove;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoFruCapability::NoFruCapability(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoFruCapability::getName() {
    return "NoFruCapability";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoFruCapability::getDescription() {
    return "the resource is not associated with a FRU, as indicated by"
           "SAHPI_CAPABILITY_FRU in the ResourceCapabilities field of the"
	   " resource's RPT entry.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoFruCapability::getPrecondition() {
    return "Requires one or more resources without the FRU capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoFruCapability::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoFruCapability::runResourceTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (hasFruCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        
        SaErrorT error = saHpiResourceFailedRemove(sessionId,
                                                   rptEntry->ResourceId);
	if (error == SA_ERR_HPI_INVALID_CMD) {
	  status.assertPass();
        } else if ((error == SA_ERR_HPI_INVALID_REQUEST) && 
		   (!rptEntry->ResourceFailed)) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RESOURCE_FAILED_REMOVE,
				 SA_ERR_HPI_INVALID_CMD, error);
        }
    }

    return status;
}


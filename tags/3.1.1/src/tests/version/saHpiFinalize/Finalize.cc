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

#include "Finalize.h"

using namespace ns_saHpiFinalize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Finalize::Finalize(char *line) : HpiLibraryTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Finalize::getName() {
    return "Finalize";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Finalize::getDescription() {
    return "Initialize the library and verify if a session is closed by saHpiFinalize().";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Finalize::runLibraryTest() {
    HpiTestStatus status;

	// We have to make sure that the library is initialized    
	SaErrorT error = saHpiInitialize(SAHPI_INTERFACE_VERSION, 0, NULL, NULL, NULL);
	if (error != SA_OK) {
		status.assertError(TRACE, INITIALIZE, SA_OK, error);
	}
	
	// Generate a new session 
	SaHpiSessionIdT sessionId;
    error = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID, &sessionId, NULL);
	if (error != SA_OK) {
        status.assertFailure(TRACE, SESSION_OPEN, SA_OK, error);
    }
	
	error = saHpiFinalize();
	if (error != SA_OK) {
		status.assertFailure(TRACE, FINALIZE, SA_OK, error);
	} 

	// Check whether the Session is already closed
	error = saHpiSessionClose(sessionId);
	if (error == SA_ERR_HPI_INVALID_SESSION) {
		status.assertPass();
    } else {
        status.assertFailure(TRACE, SESSION_CLOSE, 
                             SA_ERR_HPI_INVALID_SESSION, error);
    } 
	
    return status;
}


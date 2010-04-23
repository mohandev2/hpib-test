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

#include "InvalidRequest.h"

using namespace ns_saHpiInitialize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidRequest::InvalidRequest(char *line) : HpiLibraryTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidRequest::getName() {
    return "InvalidRequest";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidRequest::getDescription() {
    return "The HPI library is not in initial state.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidRequest::runLibraryTest() {
    HpiTestStatus status;

	SaErrorT error = saHpiInitialize(SAHPI_INTERFACE_VERSION, 0, 
									NULL, NULL, NULL);
    	if (error != SA_ERR_HPI_INVALID_REQUEST) {
    		status.assertFailure(TRACE, INITIALIZE, SA_ERR_HPI_INVALID_REQUEST, error);
    } else {
    		status.assertPass();
    }
    return status;
}


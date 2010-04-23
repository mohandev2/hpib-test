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

using namespace ns_saHpiFinalize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidRequest::InvalidRequest(char *line) : HpiGenTestCase(line) {
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
    return "Call saHpiFinalize() without initializing the library before.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidRequest::run() {
    HpiTestStatus status;

    SaErrorT error = saHpiFinalize();
    if (error == SA_ERR_HPI_INVALID_REQUEST) { 
    		status.assertPass();
    } else if (error == SA_OK) {
    		error = saHpiFinalize();
    		if (error != SA_ERR_HPI_INVALID_REQUEST) {
    			status.assertFailure(TRACE, FINALIZE, SA_ERR_HPI_INVALID_REQUEST, error);
    		}
    } else {
    		status.assertFailure(TRACE, FINALIZE, SA_ERR_HPI_INVALID_REQUEST, error);
    }
    return status;
}


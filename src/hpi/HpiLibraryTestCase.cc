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
 
#include "HpiTestStatus.h"
#include "HpiLibraryTestCase.h"
#include "LocationMgr.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiLibraryTestCase::HpiLibraryTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiLibraryTestCase::HpiLibraryTestCase(char *line) : HpiTestCase(line) { 
}

/*****************************************************************************
 * Test library in initial state.
 *****************************************************************************/

TestStatus HpiLibraryTestCase::runTest() {
    HpiTestStatus status;
	SaErrorT error;
	
	error = saHpiFinalize();
	if ((error == SA_OK) || (error == SA_ERR_HPI_INVALID_REQUEST)) {
	    status = runLibraryTest();
	} else {
	 	 status.assertFailure(TRACE, FINALIZE, SA_ERR_HPI_INVALID_REQUEST, error);  
	}
    return status;
}


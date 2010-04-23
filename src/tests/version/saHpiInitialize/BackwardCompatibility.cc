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

#include "BackwardCompatibility.h"

using namespace ns_saHpiInitialize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

BackwardCompatibility::BackwardCompatibility(char *line) : HpiLibraryTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *BackwardCompatibility::getName() {
    return "BackwardCompatibility";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *BackwardCompatibility::getDescription() {
    return "Check the backward compatibility of the library without any options.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus BackwardCompatibility::runLibraryTest() {
    HpiTestStatus status;
    int allVersions = 3;
    SaHpiVersionT versions[allVersions];
	
	versions[0] = 0x020101;
 	versions[1] = 0x020201;
 	versions[2] = 0x020301;
 	
 	for (int i=0; i<allVersions; i++) {
		SaErrorT error = saHpiInitialize(versions[i], 0, NULL, NULL, NULL);
		if (error != SA_OK) {
			status.assertFailure(TRACE, INITIALIZE, SA_OK, error,
				"Failed for version 0x%6X\n", versions[i]);
		} else {	
			status.assertPass();
    		}
    		
    		error = saHpiFinalize();
    		if (error != SA_OK) {
    			status.assertFailure(TRACE, FINALIZE, SA_OK, error);	
    		}
 	} 
	
    return status;
}


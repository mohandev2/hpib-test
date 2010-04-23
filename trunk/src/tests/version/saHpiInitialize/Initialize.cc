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

#include "Initialize.h"

using namespace ns_saHpiInitialize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Initialize::Initialize(char *line) : HpiLibraryTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Initialize::getName() {
    return "Initialize";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Initialize::getDescription() {
    return "Initialize the library without any options.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Initialize::runLibraryTest() {
    HpiTestStatus status;

	SaErrorT error = saHpiInitialize(SAHPI_INTERFACE_VERSION, 0, NULL, NULL, NULL);
	if (error != SA_OK) {
		status.assertError(TRACE, INITIALIZE, SA_OK, error);
	} else {	
		status.assertPass();
    } 
	
    return status;
}


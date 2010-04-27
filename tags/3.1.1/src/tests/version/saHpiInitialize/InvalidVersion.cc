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

#include "InvalidVersion.h"

using namespace ns_saHpiInitialize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidVersion::InvalidVersion(char *line) : HpiLibraryTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidVersion::getName() {
    return "InvalidVersion";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidVersion::getDescription() {
    return "The HPI Library does not support the RequestedVersion.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidVersion::getExpectedReturn() {
    return SA_ERR_HPI_UNSUPPORTED_API;
}


/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidVersion::runLibraryTest() {
    HpiTestStatus status;

	SaErrorT error = saHpiInitialize(getInvalidVersion(), 0, 
									 NULL, NULL, NULL);
    if (error == SA_ERR_HPI_UNSUPPORTED_API) {
    		status.assertPass();
    } else {
    		status.assertFailure(TRACE, INITIALIZE, SA_ERR_HPI_UNSUPPORTED_API, error);
    }
    return status;
}


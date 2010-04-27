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

#include "GetVersion.h"

using namespace ns_saHpiVersionGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetVersion::GetVersion(char *line) : HpiGenTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetVersion::getName() {
    return "GetVersion";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetVersion::getDescription() {
    return "Verify that the returned version is equivalent to SAHPI_INTERFACE_VERSION.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetVersion::run() {
    HpiTestStatus status;

    SaHpiVersionT version = saHpiVersionGet();
    if (version == SAHPI_INTERFACE_VERSION) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, "Invalid version: %x", version);
    } 

    return status;
}


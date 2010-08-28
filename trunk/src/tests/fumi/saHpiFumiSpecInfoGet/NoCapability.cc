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
 *     Meghana Srivatsavai <mjn77@unh.edu>
 * 
 * Changes:
 * 09/11/27 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Integrate the test case into hpitest 
 */

#include "NoCapability.h"

using namespace ns_saHpiFumiSpecInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoCapability::NoCapability(char *line)
: HpiNoCapabilityTestCase(line, "Fumi") {
}

/*****************************************************************************
 * Return the precondition for this test case.
 *****************************************************************************/

const char *NoCapability::getPrecondition() {
    return "Requires a resource that does not support Firmware Upgrade management Instruments.";
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoCapability::runResourceTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiFumiSpecInfoT info;

    if (hasFumiCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiFumiSpecInfoGet(sessionId, rptEntry->ResourceId,
                                              getValidFumiNum(), &info);
        if (error == SA_ERR_HPI_CAPABILITY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, FUMI_SPEC_INFO_GET, 
                                 SA_ERR_HPI_CAPABILITY, error);
        } 
    }

    return status;
}


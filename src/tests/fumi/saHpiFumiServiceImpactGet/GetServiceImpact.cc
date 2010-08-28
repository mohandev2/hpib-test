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

#include "GetServiceImpact.h"

using namespace ns_saHpiFumiServiceImpactGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetServiceImpact::GetServiceImpact(char *line) : FumiTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetServiceImpact::getName() {
    return "GetServiceImpact";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetServiceImpact::getDescription() {
    return "Used to obtain information about the potential service impact \n"
           "of an upgrade process on a FUMI.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetServiceImpact::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetServiceImpact::runFumiTest(SaHpiSessionIdT sessionId, 
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiFumiRecT *fumiRec) {    
    HpiTestStatus status;
    SaHpiFumiServiceImpactDataT siData;
    
    SaErrorT error = saHpiFumiServiceImpactGet(sessionId,
                                              rptEntry->ResourceId,
                                              fumiRec->Num,
                                              &siData);
 if (error == SA_ERR_HPI_BUSY) {
        status.assertNotSupport();
    } else if (error == SA_ERR_HPI_ENTITY_NOT_PRESENT) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertFailure(TRACE, FUMI_SERVICE_IMPACT_GET, SA_OK, error);
    } else {
        status.assertPass();
    }

    return status;
}

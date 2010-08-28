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

#include "NullServiceImpact.h"

using namespace ns_saHpiFumiServiceImpactGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullServiceImpact::NullServiceImpact(char *line) : FumiTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullServiceImpact::getName() {
    return "NullServiceImpact";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullServiceImpact::getDescription() {
    return "The <i>Service Impact</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullServiceImpact::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullServiceImpact::runFumiTest(SaHpiSessionIdT sessionId, 
                                    SaHpiRptEntryT *rptEntry,
                                    SaHpiFumiRecT *fumiRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiFumiServiceImpactGet(sessionId, 
                                               rptEntry->ResourceId,
                                               fumiRec->Num, NULL);

    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, FUMI_SERVICE_IMPACT_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


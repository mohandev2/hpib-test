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
 * 09/12/28 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Integrate the test case into hpitest 
 */

#include "NullDisable.h"

using namespace ns_saHpiFumiAutoRollbackDisableGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullDisable::NullDisable(char *line) : FumiTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullDisable::getName() {
    return "NullDisable";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullDisable::getDescription() {
    return "The <i>Disable</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullDisable::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullDisable::runFumiTest(SaHpiSessionIdT sessionId,
                                   SaHpiRptEntryT *rptEntry,SaHpiFumiRecT *fumiRec) {
    HpiTestStatus status;

    SaErrorT error = saHpiFumiAutoRollbackDisableGet(sessionId, rptEntry->ResourceId,
                                                     fumiRec->Num, NULL);

    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, FUMI_AUTO_ROLLBACK_DISABLE_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


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

#include "ResetTestCase.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_BUSY_RETRY 8

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ResetTestCase::ResetTestCase() : HpiResourceTestCase() {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ResetTestCase::ResetTestCase(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Run the test only if the resource supports Reset.
 *****************************************************************************/

HpiTestStatus ResetTestCase::runResourceTest(SaHpiSessionIdT sessionId,
                                             SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasResetCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        status = runResetTest(sessionId, rptEntry);
    }

    return status;
}

/*****************************************************************************
 * Get the reset state of the resource.  If the resource is busy, then keep
 * trying a number times.
 *****************************************************************************/

SaErrorT ResetTestCase::getResetState(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiResetActionT *resetAction) {
    int busyCount = 0;

    SaErrorT error = saHpiResourceResetStateGet(sessionId, resourceId, resetAction);
    while (error == SA_ERR_HPI_BUSY && busyCount < MAX_BUSY_RETRY) {
        busyCount++;
        secSleep(1);
        error = saHpiResourceResetStateGet(sessionId, resourceId, resetAction);
    }

    return error;
}

/*****************************************************************************
 * Set the reset state of the resource.  If the resource is busy, then keep
 * trying a number times.
 *****************************************************************************/

SaErrorT ResetTestCase::setResetState(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId,
                                      SaHpiResetActionT resetAction) {
    int busyCount = 0;

    SaErrorT error = saHpiResourceResetStateSet(sessionId, resourceId, resetAction);
    while (error == SA_ERR_HPI_BUSY && busyCount < MAX_BUSY_RETRY) {
        busyCount++;
        secSleep(1);
        error = saHpiResourceResetStateSet(sessionId, resourceId, resetAction);
    }

    return error;
}

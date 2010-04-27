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

#include "OverflowNotResetable.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogOverflowReset;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OverflowNotResetable::OverflowNotResetable(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OverflowNotResetable::getName() {
    return "OverflowNotResetable";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OverflowNotResetable::getDescription() {
    return "If the <i>OverflowResetable</i> flag in the Event Log info record\n"
           "is False, then the implementation does not support reseting\n"
           "the <i>OverflowFlag</i>.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OverflowNotResetable::getPrecondition() {
    return "Requires at least one Event Log whose Overflow flag cannot be cleared.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OverflowNotResetable::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_CMD;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus OverflowNotResetable::runAddTest(SaHpiSessionIdT sessionId,
                                               SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.OverflowResetable) {
        status.assertNotSupport();
    } else {
        status.add(TRACE, EventLogHelper::overflow(sessionId, resourceId));
        if (status.isOkay()) {
            error = saHpiEventLogOverflowReset(sessionId, resourceId);
            if (error == SA_ERR_HPI_INVALID_CMD) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, EVENT_LOG_OVERFLOW_RESET,
                                     SA_ERR_HPI_INVALID_CMD, error);
            } 
        }

        // since we filled up the eventlog, let's clear it for other tests

        error = saHpiEventLogClear(sessionId, resourceId);
        status.checkError(TRACE, EVENT_LOG_CLEAR, error);
    }

    return status;
}


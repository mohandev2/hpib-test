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

#include "OverflowFlag.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogClear;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OverflowFlag::OverflowFlag(char *line) : EventLogTestCase(DISABLE_LOG, line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OverflowFlag::getName() {
    return "OverflowFlag";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OverflowFlag::getDescription() {
    return "The <i>OverflowFlag</i> in the Event Log info record will be\n"
           "reset when this function is called.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OverflowFlag::getPrecondition() {
    return "Requires a Event Log that does not have a size of zero.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OverflowFlag::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus OverflowFlag::runLogTest(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    status.add(TRACE, EventLogHelper::overflow(sessionId, resourceId));
    if (status.isOkay()) {
        SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
        } else if (!info.OverflowFlag) {
            status.assertError(TRACE,
                    "OverflowFlag is not set despite overflowing the Event Log.");
        } else {
            error = saHpiEventLogClear(sessionId, resourceId);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_CLEAR, SA_OK, error);
            } else {
                error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
                if (error != SA_OK) {
                    status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
                } else if (info.OverflowFlag) {
                    status.assertFailure(TRACE,
                        "OverflowFlag was not reset after calling saHpiEventLogClear().");
                } else {
                    status.assertPass();
                }
            }
        }
    }

    return status;
}


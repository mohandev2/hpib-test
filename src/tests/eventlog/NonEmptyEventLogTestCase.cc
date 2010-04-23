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

#include "NonEmptyEventLogTestCase.h"
#include "EventLogHelper.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NonEmptyEventLogTestCase::NonEmptyEventLogTestCase(char *line)
: EventLogTestCase(DISABLE_LOG, line) {
    this->forceAdd = false;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NonEmptyEventLogTestCase::NonEmptyEventLogTestCase(bool newForceAdd, char *line)
: EventLogTestCase(DISABLE_LOG, line) {
    this->forceAdd = newForceAdd;
}

/*****************************************************************************
 * Make sure that the EventLog is not empty.  If it is empty or forceAdd has
 * been set, then add an entry to the EventLog before running the test.
 *****************************************************************************/

HpiTestStatus NonEmptyEventLogTestCase::runLogTest(SaHpiSessionIdT sessionId,
                                                   SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.Size == 0) {
        status.assertNotSupport();
    } else {
        if (info.Entries != 0 && !forceAdd) {
            status.add(TRACE, runNonEmptyTest(sessionId, resourceId));
        } else {
            status.add(TRACE, EventLogHelper::addEntry(sessionId, resourceId));
            if (status.isOkay()) {
                status.add(TRACE, runNonEmptyTest(sessionId, resourceId));
            }
        }
    }

    return status;
}

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

#include "Clear.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogClear;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

Clear::Clear(char *line) : NonEmptyEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *Clear::getName() {
    return "Clear";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *Clear::getDescription() {
    return "Clear the Event Log and verify that it is empty.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *Clear::getPrecondition() {
    return "Requires an Event Log with entries or the ability to add entries.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT Clear::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus Clear::runNonEmptyTest(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    SaErrorT error = saHpiEventLogClear(sessionId, resourceId);
    if (error != SA_OK) {
        status.assertFailure(TRACE, EVENT_LOG_CLEAR, SA_OK, error);
    } else {
        bool isEmpty;
        status.add(TRACE, EventLogHelper::isEmpty(sessionId, resourceId, &isEmpty));
        if (status.isOkay()) {
            if (isEmpty) {
                status.assertPass();
            } else {
                status.assertFailure(TRACE, 
                        "The EventLog is not empty after clearing it!");
            }
        }
    }

    return status;
}

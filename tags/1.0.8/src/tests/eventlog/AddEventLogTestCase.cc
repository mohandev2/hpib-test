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

#include "AddEventLogTestCase.h"
#include "EventLogHelper.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddEventLogTestCase::AddEventLogTestCase(char *line)
: EventLogTestCase(DISABLE_LOG, line) {
    this->available = 1;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddEventLogTestCase::AddEventLogTestCase(SaHpiUint32T newAvailable, char *line) 
: EventLogTestCase(DISABLE_LOG, line) {
    this->available = newAvailable;
}

/*****************************************************************************
 * Check to be sure that we can add an event log and then run the test.
 *****************************************************************************/

HpiTestStatus AddEventLogTestCase::runLogTest(SaHpiSessionIdT sessionId,
                                              SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    status.add(TRACE, EventLogHelper::makeRoom(sessionId, resourceId, available));
    if (status.isOkay()) {
        status.add(TRACE, runAddTest(sessionId, resourceId));
    }

    return status;
}

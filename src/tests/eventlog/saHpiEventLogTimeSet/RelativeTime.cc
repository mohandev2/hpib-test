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
 *
 * Changes     
 * 2009/05/19 - Lars.Wetzel@emerson.com
 *              Verify if proper capability is set 
 */

#include "RelativeTime.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogTimeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

RelativeTime::RelativeTime(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *RelativeTime::getName() {
    return "RelativeTime";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *RelativeTime::getDescription() {
    return "Set the relative time and verify that the change was successful.\n"
           "Also, verify that a newly added entry to the Event Log will\n"
           "have a corresponding timestamp.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *RelativeTime::getPrecondition() {
    return "Requires the ability to add an entry to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT RelativeTime::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 * 
 * Change the time to the half the relative time and verify that the
 * change did occur.  Also, test whether a user event that is added to the
 * Event Log is assigned this new time.
 *****************************************************************************/

HpiTestStatus RelativeTime::runAddTest(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiTimeT time;
    SaHpiTimeT origTime;
    SaHpiTimeT newTime = (SAHPI_TIME_MAX_RELATIVE >> 1);

    SaErrorT error = saHpiEventLogTimeGet(sessionId, resourceId, &origTime);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_TIME_GET, SA_OK, error);
    } else {
        TimeMsec startTime = getCurrentTime();
        error = saHpiEventLogTimeSet(sessionId, resourceId, newTime);
	if ((error == SA_ERR_HPI_INVALID_CMD) &&
	    (!EventLogHelper::hasEvtLogTimerSetCapability(sessionId, resourceId))) {
	  status.assertNotSupport();

	} else if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_TIME_SET, SA_OK, error);

        } else {
            error = saHpiEventLogTimeGet(sessionId, resourceId, &time);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_TIME_GET, SA_OK, error);

            } else if (isNanoTimeEqual(time, newTime)) {
                status.add(TRACE, testUserEvent(sessionId, resourceId, time));

	    } else if (!EventLogHelper::hasEvtLogTimerSetCapability(sessionId,resourceId)) {
	      status.assertFailure(TRACE, "Function is supported but CAPABILITY isn't set\n");

            } else {
                status.assertFailure(TRACE, 
                        "Check of new time failed to get a time that was "
                        "slightly greater than or equal to time that was set.\n"
                        "Time = %lld", time);
            }

            // add any elapsed to the original time
            TimeMsec endTime = getCurrentTime();
            origTime += ((endTime - startTime) * 1000000);

            // Restore the time
            error = saHpiEventLogTimeSet(sessionId, resourceId, origTime);
            status.checkError(TRACE, EVENT_LOG_TIME_SET, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Add a user event to the Event Log and verify that it's timestamp 
 * corresponds to the given startTime.
 *****************************************************************************/

HpiTestStatus RelativeTime::testUserEvent(SaHpiSessionIdT sessionId,
                                          SaHpiResourceIdT resourceId,
                                          SaHpiTimeT startTime) {
    HpiTestStatus status;
    SaHpiEventLogEntryIdT prevEntryId;
    SaHpiEventLogEntryIdT nextEntryId;
    SaHpiEventLogEntryT eventLogEntry;

    status.add(TRACE, EventLogHelper::addEntry(sessionId, resourceId));
    if (status.isOkay()) {
        SaErrorT error = saHpiEventLogEntryGet(sessionId, resourceId,
                                               SAHPI_NEWEST_ENTRY, 
                                               &prevEntryId, &nextEntryId,
                                               &eventLogEntry, NULL, NULL);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_ENTRY_GET, SA_OK, error);
        } else {
            SaHpiTimeT timestamp = eventLogEntry.Timestamp;
            if (!isNanoTimeEqual(timestamp, startTime)) {
                status.assertFailure(TRACE, "User Event has the wrong timestamp %lld!",
                                  timestamp);
            } else {
                status.assertPass();
            }
        }
    }

    return status;
}

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

#include <time.h>
#include <sys/time.h>
#include "AbsoluteTime.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogTimeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AbsoluteTime::AbsoluteTime(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AbsoluteTime::getName() {
    return "AbsoluteTime";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AbsoluteTime::getDescription() {
    return "Set the absolute time and verify that the change was successful. "
           "Also, verify that a newly added entry to the Event Log will "
           "have a corresponding timestamp.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AbsoluteTime::getPrecondition() {
    return "Requires the ability to add an entry to an Event Log.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AbsoluteTime::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 * 
 * Change the time to the very start of absolute time and verify that the
 * change did occur.  Also, test whether a user event that is added to the
 * Event Log is assigned this new time.
 *****************************************************************************/

HpiTestStatus AbsoluteTime::runAddTest(SaHpiSessionIdT sessionId,
                                       SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiTimeT time;
    SaHpiTimeT origTime;
    SaHpiTimeT newTime = getNewTime();

    SaErrorT error = saHpiEventLogTimeGet(sessionId, resourceId, &origTime);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_TIME_GET, SA_OK, error);
    } else {
        TimeMsec startTime = getCurrentTime();
        error = saHpiEventLogTimeSet(sessionId, resourceId, newTime);
        if (error != SA_OK) {
            status.assertError(TRACE, EVENT_LOG_TIME_SET, SA_OK, error);
        } else {
            error = saHpiEventLogTimeGet(sessionId, resourceId, &time);
            if (error != SA_OK) {
                status.assertError(TRACE, EVENT_LOG_TIME_GET, SA_OK, error);
            } else if (isNanoTimeEqual(time, newTime)) {
                status.add(TRACE, testUserEvent(sessionId, resourceId, time));
            } else {
                status.assertFailure(TRACE,
                    "Check of new time failed to get a time that was "
                    "equal to the time that was set. Time = %lld", time);
            }

            // add any elapsed time to the original time
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

HpiTestStatus AbsoluteTime::testUserEvent(SaHpiSessionIdT sessionId,
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
                status.assertFailure(TRACE, 
                        "User Event has the wrong timestamp %lld!", timestamp);
            } else {
                status.assertPass();
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Get a time to use for the Event Log.  Go back 24 hours in time from 
 * the current time.
 *****************************************************************************/

SaHpiTimeT AbsoluteTime::getNewTime() {
    struct timeval tv1;
    
    gettimeofday(&tv1, NULL);
    tv1.tv_sec -= 86400; // subtract one day
    SaHpiTimeT time = (SaHpiTimeT) tv1.tv_sec * 1000000000 + tv1.tv_usec * 1000;

    return time;
}


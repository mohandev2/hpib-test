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

#include "DataLength.h"
#include "EventHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

DataLength::DataLength(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *DataLength::getName() {
    return "DataLength";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *DataLength::getDescription() {
    return "The event <i>DataLength</i> is larger than that supported by the\n"
           "implementation and reported in the Event Log info record.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *DataLength::getPrecondition() {
    return "The info record field UserEventMaxSize must be\n"
           "less than SAHPI_MAX_TEXT_BUFFER_LENGTH.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT DataLength::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test
 *****************************************************************************/

HpiTestStatus DataLength::runAddTest(SaHpiSessionIdT sessionId,
                                     SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.UserEventMaxSize >= SAHPI_MAX_TEXT_BUFFER_LENGTH) {
        // We are only using text buffers for this event
        // This test could only work if the UserEventMaxSize
        // is less than the maximum text buffer size.
        status.assertNotSupport();
    } else {

        EventHelper::fill(&event, info.UserEventMaxSize + 1);

        error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD,
                                 SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}


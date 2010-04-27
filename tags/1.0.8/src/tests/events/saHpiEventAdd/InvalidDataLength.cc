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

#include "InvalidDataLength.h"
#include "EventHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidDataLength::InvalidDataLength(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidDataLength::getName() {
    return "InvalidDataLength";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidDataLength::getDescription() {
    return "The amount of event data is greater than that\n"
           "specified by <i>UserEventMaxSize</i> in the domain info record.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidDataLength::getPrecondition() {
    return "Requires a Domain Event Log with a <i>UserEventMaxSize</i>\n"
           "that is less than SAHPI_MAX_TEXT_BUFFER_LENGTH.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidDataLength::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidDataLength::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventLogInfoT info;
    SaHpiTextBufferT buf;

    SaErrorT error = saHpiEventLogInfoGet(sessionId,
                                          SAHPI_UNSPECIFIED_RESOURCE_ID, &info);
    if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else if (info.UserEventMaxSize >= SAHPI_MAX_TEXT_BUFFER_LENGTH) {
        status.assertNotSupport();
    } else {
        TextBufferHelper::fill(&buf, info.UserEventMaxSize + 1);
        EventHelper::fill(&event, &buf);
        error = saHpiEventAdd(sessionId, &event);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_ADD, SA_ERR_HPI_INVALID_DATA, error);
        }
    }

    return status;
}

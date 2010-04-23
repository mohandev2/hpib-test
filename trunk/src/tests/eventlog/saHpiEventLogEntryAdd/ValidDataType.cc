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

#include "ValidDataType.h"
#include "TextBufferHelper.h"
#include "EventHelper.h"
#include "EventLogHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

ValidDataType::ValidDataType(char *line) : AddEventLogTestCase(5, line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *ValidDataType::getName() {
    return "ValidDataType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *ValidDataType::getDescription() {
    return "Test adding event log entries with all of the valid DataTypes.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *ValidDataType::getPrecondition() {
    return "Must be able to add an entry to an Event Log without\n"
           "the error code SA_ERR_HPI_INVALID_DATA being returned.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT ValidDataType::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus ValidDataType::runAddTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiTextBufferT buf;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE, 
                                  SAHPI_TL_TYPE_TEXT, 
                                  SAHPI_TL_TYPE_ASCII6, 
                                  SAHPI_TL_TYPE_BCDPLUS, 
                                  SAHPI_TL_TYPE_BINARY };

    if (EventLogHelper::hasEvtLogAddCapability(sessionId, resourceId)) { 
      for (int i = 0; i < 5; i++) {
        TextBufferHelper::fillByDataType(&buf, dataType[i]);
        EventHelper::fill(&event, &buf);

        SaErrorT error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
        if (error == SA_ERR_HPI_INVALID_DATA) {
            status.assertNotSupport();
        } else if (error == SA_OK) {
	  status.assertPass();
        } else {
	  status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD, SA_OK, error);
        }
      }
    }
    return status;
}

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
 */

#include "OverflowOverwrite.h"
#include "EventHelper.h"
#include "EventLogHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

OverflowOverwrite::OverflowOverwrite(char *line) : AddEventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *OverflowOverwrite::getName() {
    return "OverflowOverwrite";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *OverflowOverwrite::getDescription() {
    return "If the Event Log's <i>OverflowAction</i> is SAHPI_EL_OVERFLOW_OVERWRITE,\n"
           "events can be added to the Event Log, but another event entry will\n"
           "be discarded.  The <i>OverflowFlag</i> is also checked to verify that it\n"
           "was set for an overflow.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *OverflowOverwrite::getPrecondition() {
    return "Requires an Event Log with an <i>OverflowAction</i>\n"
           "of SAHPI_EL_OVERFLOW_OVERWRITE.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT OverflowOverwrite::getExpectedReturn() {
    return SA_ERR_HPI_OUT_OF_SPACE;
}

/*****************************************************************************
 * Run the test.
 *
 * First, clear the log in order in order to clear the OverflowFlag.  Then
 * fill up the Event Log.  Try adding a new entry.  It should fail.  Then
 * verify that it really wasn't added.  Lastly, verify that the OverflowFlag
 * was set.
 *****************************************************************************/

HpiTestStatus OverflowOverwrite::runAddTest(SaHpiSessionIdT sessionId,
                                            SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiEventLogInfoT info;
    SaHpiTextBufferT text;

    //    TextBufferHelper::fill(&text, "OverflowOverwrite test");
    status.add(TRACE, prepareTestData(sessionId, resourceId, &text));

    if (status.isOkay() &&
	EventLogHelper::hasEvtLogClearCapability(sessionId, resourceId)) {
      SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
      if (error != SA_OK) {
        status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
     
      } else if (info.OverflowAction != SAHPI_EL_OVERFLOW_OVERWRITE) {
        status.assertNotSupport();

      } else {
        error = saHpiEventLogClear(sessionId, resourceId);
        if (error != SA_OK) {
	  status.assertError(TRACE, EVENT_LOG_CLEAR, SA_OK, error);
        } else  {
	  status.add(TRACE, EventLogHelper::fill(sessionId, resourceId));
	  if (status.isOkay()) {

	    EventHelper::fill(&event, &text);
	    error = saHpiEventLogEntryAdd(sessionId, resourceId, &event);
	    if (error != SA_OK) {
	      status.assertFailure(TRACE, EVENT_LOG_ENTRY_ADD, SA_OK, error);

	    } else {
	      bool found;
	      status.add(TRACE, EventLogHelper::findEvent(sessionId, resourceId,
                                                                &event, &found));
	      if (status.isOkay() && !found) {
		status.assertFailure(TRACE, "The event was not added.");
	      }

	      error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
	      if (error != SA_OK) {
		status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
	      } else {
		status.assertPass();
		if (!info.OverflowFlag) {
		  status.assertFailure(TRACE, "OverflowFlag was not set.");
		}
		if (info.Entries > info.Size) {
		  status.assertFailure(TRACE,
				       "More entries than can fit into the EventLog.");
		}
	      }
	    }

	    // since will filled up the Event Log, let's clear it for the following tests.
	    error = saHpiEventLogClear(sessionId, resourceId);
	    status.checkError(TRACE, EVENT_LOG_CLEAR, error);
	  }
        }
      }
    }
    return status;
}


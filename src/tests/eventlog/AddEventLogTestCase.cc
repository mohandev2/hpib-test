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
 * 2009/05/25 - lars.wetzel@emerson.com
 *              Add prepareTestData()
 */

#include "AddEventLogTestCase.h"
#include "TextBufferHelper.h"
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

HpiTestStatus AddEventLogTestCase::prepareTestData(SaHpiSessionIdT sessionId,
						   SaHpiResourceIdT resourceId,
						   SaHpiTextBufferT *buffer) {
  HpiTestStatus status;
  SaHpiEventLogInfoT info;

  // Check if the event log supports adding using events
  if (!EventLogHelper::hasEvtLogAddCapability(sessionId, resourceId)) {
    status.assertNotSupport();

  } else {
    // Check if the buffer data size is supported by the event log
    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error != SA_OK) {
      status.assertError(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } else {
      if (info.UserEventMaxSize < SAHPI_MAX_TEXT_BUFFER_LENGTH) {
	TextBufferHelper::fill(buffer, info.UserEventMaxSize); 
      } else {
	TextBufferHelper::fill(buffer, SAHPI_MAX_TEXT_BUFFER_LENGTH);
      }
      status.assertPass();
    }
  
  }

  return status;
}

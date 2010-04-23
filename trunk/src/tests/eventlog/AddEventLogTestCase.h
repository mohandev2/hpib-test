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

#ifndef __ADDEVENTLOGTESTCASE_H__
#define __ADDEVENTLOGTESTCASE_H__

#include "SafTest.h"
#include "EventLogTestCase.h"

/*****************************************************************************
 * Add EventLog Test Case
 *
 * This base class is used when it is necessary to be able to add event log
 * entries to the event log.
 *****************************************************************************/

class AddEventLogTestCase : public EventLogTestCase
{
private:
    SaHpiUint32T available;

 protected:
    HpiTestStatus prepareTestData(SaHpiSessionIdT sessionId,
				  SaHpiResourceIdT resourceId,
				  SaHpiTextBufferT *buffer);
public:
    AddEventLogTestCase(char *line);
    AddEventLogTestCase(SaHpiUint32T available, char *line);

    HpiTestStatus runLogTest(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId);

    virtual HpiTestStatus runAddTest(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId) = 0;
};

#endif

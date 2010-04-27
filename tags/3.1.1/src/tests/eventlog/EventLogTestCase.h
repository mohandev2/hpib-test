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

#ifndef __EVENTLOGTESTCASE_H__
#define __EVENTLOGTESTCASE_H__

#include "SafTest.h"
#include "HpiSessionTestCase.h"

/*****************************************************************************
 * Actions for the event log.
 *****************************************************************************/

#define NULL_LOG     0    // do nothing
#define DISABLE_LOG  1    // disable the log
#define ENABLE_LOG   2    // enable the log

/*****************************************************************************
 * EventLog Test Case
 *****************************************************************************/

class EventLogTestCase : public HpiSessionTestCase
{
private:
    int action;

public:
    EventLogTestCase(int action, char* line);
    EventLogTestCase(char* line);

protected:
    HpiTestStatus runSessionTest(SaHpiSessionIdT sessionId);

    virtual HpiTestStatus runLogTest(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId) = 0;

private:
    HpiTestStatus testResource(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId);

};

#endif

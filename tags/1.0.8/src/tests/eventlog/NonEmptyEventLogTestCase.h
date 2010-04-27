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

#ifndef __NONEMPTYEVENTLOGTESTCASE_H__
#define __NONEMPTYEVENTLOGTESTCASE_H__

#include "SafTest.h"
#include "EventLogTestCase.h"

/*****************************************************************************
 * Non-Empty EventLog Test Case
 *
 * This base class is used when we need an event log that isn't empty.
 *****************************************************************************/

class NonEmptyEventLogTestCase : public EventLogTestCase
{
private:
    bool forceAdd;

public:
    NonEmptyEventLogTestCase(char *line);
    NonEmptyEventLogTestCase(bool forceAdd, char *line);

    HpiTestStatus runLogTest(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId);

protected:
    virtual HpiTestStatus runNonEmptyTest(SaHpiSessionIdT sessionId, SaHpiResourceIdT resourceId) = 0;
};

#endif

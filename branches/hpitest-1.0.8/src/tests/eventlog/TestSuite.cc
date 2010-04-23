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

#include "TestSuite.h"
#include "saHpiEventLogInfoGet/TestSuite.h"
#include "saHpiEventLogEntryGet/TestSuite.h"
#include "saHpiEventLogEntryAdd/TestSuite.h"
#include "saHpiEventLogClear/TestSuite.h"
#include "saHpiEventLogTimeGet/TestSuite.h"
#include "saHpiEventLogTimeSet/TestSuite.h"
#include "saHpiEventLogStateGet/TestSuite.h"
#include "saHpiEventLogStateSet/TestSuite.h"
#include "saHpiEventLogOverflowReset/TestSuite.h"

using namespace ns_eventlog;

/*****************************************************************************
 * Constructore
 *****************************************************************************/

TestSuite::TestSuite() { 
    add(new ns_saHpiEventLogInfoGet::TestSuite());
    add(new ns_saHpiEventLogEntryGet::TestSuite());
    add(new ns_saHpiEventLogEntryAdd::TestSuite());
    add(new ns_saHpiEventLogClear::TestSuite());
    add(new ns_saHpiEventLogTimeGet::TestSuite());
    add(new ns_saHpiEventLogTimeSet::TestSuite());
    add(new ns_saHpiEventLogStateGet::TestSuite());
    add(new ns_saHpiEventLogStateSet::TestSuite());
    add(new ns_saHpiEventLogOverflowReset::TestSuite());
}

/*****************************************************************************
 * Return a name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "EventLog";
}

/*****************************************************************************
 * Return a description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Event Log Management.";
}

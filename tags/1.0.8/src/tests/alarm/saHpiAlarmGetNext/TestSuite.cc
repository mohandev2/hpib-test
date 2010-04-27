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
#include "AddAlarm.h"
#include "DeletedAlarm.h"
#include "GetAllAlarms.h"
#include "MismatchedTimestamp.h"
#include "InvalidSeverity.h"
#include "NullAlarm.h"
#include "Order.h"
#include "GetBySeverity.h"
#include "UnacknowledgedOnly.h"
#include "GetUnacknowledgedBySeverity.h"
#include "InvalidSession.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddAlarm("P67-20:P67-20"));
    add(new NullAlarm("P67-23:P67-23"));
    add(new Order("P67-32:P67-32"));
    add(new MismatchedTimestamp("P67-29:P67-30"));
    add(new DeletedAlarm("P67-34:P67-35"));

    add(new GetBySeverity("P67-37:P67-37"));
    add(new UnacknowledgedOnly("P67-37:P67-37"));
    add(new GetUnacknowledgedBySeverity("P67-37:P67-37"));

    add(new GetAllAlarms("P68-1:P68-8"));

    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAlarmGetNext";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAlarmGetNext().";
}

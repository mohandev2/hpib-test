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
     add(new AddAlarm("B.02|P84-20:P84-20"));
    add(new NullAlarm("B.02|P84-23:P84-23"));
    add(new Order("B.02|P84-32:P84-32"));
    add(new MismatchedTimestamp("B.02|P84-29:P84-30"));
    add(new DeletedAlarm("B.02|P84-34:P84-35"));

    add(new GetBySeverity("B.02|P84-37:P84-37"));
    add(new UnacknowledgedOnly("B.02|P84-37:P84-37"));
    add(new GetUnacknowledgedBySeverity("B.02|P84-37:P84-37"));

    add(new GetAllAlarms("B.02|P84-1:P84-8"));

    add(new InvalidSession("B.02|P36-17:P36-18"));
 

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

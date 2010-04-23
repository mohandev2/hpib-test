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
#include "DeleteUserAlarm.h"
#include "InvalidSeverity.h"
#include "InvalidAlarmId.h"
#include "OnlyUserAlarms1.h"
#include "OnlyUserAlarms2.h"
#include "IgnoreSeverity.h"
#include "DeleteBySeverity.h"
#include "DeleteAllUserAlarms.h"
#include "NoSeverity.h"
#include "NonUserAlarm.h"
#include "InvalidSession.h"

using namespace ns_saHpiAlarmDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new DeleteUserAlarm("P73-17:P73-17;P73-26:P73-27"));
    add(new InvalidSeverity("P73-18:P73-19"));
    add(new InvalidAlarmId("P73-20:P73-21"));
    add(new NonUserAlarm("P73-22:P73-22"));
    add(new OnlyUserAlarms1("P73-24:P73-25"));
    add(new OnlyUserAlarms2("P73-24:P73-25"));
    add(new IgnoreSeverity("P73-27:P73-28"));
    add(new DeleteBySeverity("P73-29:P73-30"));
    add(new DeleteAllUserAlarms("P73-30:P73-31"));
    add(new NoSeverity("P73-32:P73-33"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAlarmDelete";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAlarmDelete().";
}

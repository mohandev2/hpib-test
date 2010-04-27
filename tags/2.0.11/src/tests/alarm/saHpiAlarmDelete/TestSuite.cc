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

    add(new DeleteUserAlarm("B.02|P90-17:P90-17;P90-26:P90-27"));
    add(new InvalidSeverity("B.02|P90-18:P90-19"));
    add(new InvalidAlarmId("B.02|P90-20:P90-21"));
    add(new NonUserAlarm("B.02|P90-22:P90-22"));
    add(new OnlyUserAlarms1("B.02|P90-24:P90-25"));
    add(new OnlyUserAlarms2("B.02|P90-24:P90-25"));
    add(new IgnoreSeverity("B.02|P90-27:P90-28"));
    add(new DeleteBySeverity("B.02|P90-29:P90-30"));
    add(new DeleteAllUserAlarms("B.02|P90-30:P90-31"));
    add(new NoSeverity("B.02|P90-32:P90-33"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

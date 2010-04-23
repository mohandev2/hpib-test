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
#include "NullAlarm.h"
#include "InvalidSeverity.h"
#include "InvalidAlarmCond.h"
#include "IdAndTimestamp.h"
#include "OutOfSpace.h"
#include "InvalidSession.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddAlarm("P72-12:P72-12"));
    add(new NullAlarm("P72-13:P72-13"));
    add(new InvalidSeverity("P72-14:P72-15"));
    add(new InvalidAlarmCond("P72-16:P72-17"));
    add(new OutOfSpace("P72-18:P72-19"));
    add(new IdAndTimestamp("P72-21:P72-22"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAlarmAdd";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAlarmAdd().";
}

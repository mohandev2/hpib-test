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
#include "InvalidDataType.h"
#include "InvalidLanguage.h"
#include "InvalidTextCharacters.h"
#include "IdAndTimestamp.h"
#include "OutOfSpace.h"
#include "InvalidSession.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new AddAlarm("B.02|P89-12:P89-12"));
    add(new NullAlarm("B.02|P89-13:P89-13"));
    add(new InvalidSeverity("B.02|P89-14:P89-15"));
    add(new InvalidAlarmCond("B.02|P89-16:P89-16"));
    add(new InvalidDataType("B.02|P89-16:P89-16"));
    add(new InvalidLanguage("B.02|P89-16:P89-16"));
    add(new InvalidTextCharacters("B.02|P89-16:P89-16"));
    add(new OutOfSpace("B.02|P89-23:P89-24"));
    add(new IdAndTimestamp("B.02|P89-25:P89-26"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

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
#include "AcknowledgeAlarm.h"
#include "InvalidAlarmId.h"
#include "InvalidSeverity.h"
#include "AcknowledgeBySeverity.h"
#include "AcknowledgeAllAlarms.h"
#include "AcknowledgeAlarmTwice.h"
#include "IgnoreSeverity.h"
#include "NoSeverity.h"
#include "InvalidSession.h"

using namespace ns_saHpiAlarmAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new InvalidAlarmId("P70-16:P70-17"));
    add(new InvalidSeverity("P70-18:P70-19"));
    add(new AcknowledgeAllAlarms("P70-28:P70-29"));
    add(new AcknowledgeBySeverity("P70-30:P70-31"));
    add(new AcknowledgeAlarm("P70-32:P70-34;P70-25:P70-26"));
    add(new AcknowledgeAlarmTwice("P70-35:P70-36"));
    add(new IgnoreSeverity("P70-37:P70-37"));
    add(new NoSeverity("P71-1:P71-2"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAlarmAcknowledge";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAlarmAcknowledge().";
}

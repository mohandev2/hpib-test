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
    
    add(new InvalidAlarmId("B.02|P87-16:P87-17"));	
    add(new InvalidSeverity("B.02|P87-18:P87-19"));
    add(new AcknowledgeAllAlarms("B.02|P87-28:P87-29"));
    add(new AcknowledgeBySeverity("B.02|P87-30:P87-31"));
    add(new AcknowledgeAlarm("B.02|P87-32:-34;P87-25:P87-26"));
    add(new AcknowledgeAlarmTwice("B.02|P87-35:P87-36"));
    add(new IgnoreSeverity("B.02|P87-37:P87-37"));
    add(new NoSeverity("B.02|P88-1:P88-2"));
    add(new InvalidSession("B.02|P36-17:P36-19"));

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

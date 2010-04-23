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
#include "AddEntry.h"
#include "ValidDataType.h"
#include "NoCapability.h"
#include "DataLength.h"
#include "NullEvtEntry.h"
#include "InvalidEventType.h"
#include "InvalidSeverity.h"
#include "InvalidDataType.h"
#include "InvalidDataField.h"
#include "InvalidLanguage.h"
#include "OverflowDrop.h"
#include "OverflowOverwrite.h"
#include "InvalidSession.h"
#include "InvalidResource.h"
#include "MissingCapability.h"

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
   
    add(new AddEntry("B.02|P66-16:P66-16"));
    add(new ValidDataType("B.02|P66-16:P66-16"));
    add(new NoCapability("B.02|P66-17:P66-18"));
    add(new DataLength("B.02|P66-22:P66-23"));
    add(new NullEvtEntry("B.02|P66-25:P66-25"));
    add(new InvalidEventType("B.02|P66-26:P66-27"));
    add(new InvalidSeverity("B.02|P66-28:P66-28"));
    add(new InvalidDataType("B.02|P66-31:P66-31"));
    add(new InvalidDataField("B.02|P66-32:P66-32"));
    add(new InvalidLanguage("B.02|P66-33:P66-34"));
    add(new OverflowDrop("B.02|P66-35:P66-36"));
    add(new OverflowOverwrite("B.02|P67-6:P67-10"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));
    add(new MissingCapability("B.02|P66-32:P66-32"));
    
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiEventLogEntryAdd";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiEventLogEntryAdd().";
}

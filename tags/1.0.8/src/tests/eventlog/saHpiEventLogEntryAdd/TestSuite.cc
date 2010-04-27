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

using namespace ns_saHpiEventLogEntryAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddEntry("P51-16:P51-16"));
    add(new ValidDataType("P51-16:P51-16"));
    add(new NoCapability("P51-17:P51-18"));
    add(new DataLength("P51-20:P51-21"));
    add(new NullEvtEntry("P51-23:P51-23"));
    add(new InvalidEventType("P51-24:P51-25"));
    add(new InvalidSeverity("P51-26:P51-26"));
    add(new InvalidDataType("P51-29:P51-29"));
    add(new InvalidDataField("P51-30:P51-30"));
    add(new InvalidLanguage("P51-31:P51-32"));
    add(new OverflowDrop("P51-33:P51-34"));
    add(new OverflowOverwrite("P52-6:P52-10"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
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

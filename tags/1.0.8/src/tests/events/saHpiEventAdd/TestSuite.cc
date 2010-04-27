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
#include "AddEvent.h"
#include "DomainEventLog.h"
#include "NullEvtEntry.h"
#include "InvalidEventType.h"
#include "InvalidSourceField.h"
#include "InvalidSeverity.h"
#include "InvalidDataType.h"
#include "InvalidDataField.h"
#include "InvalidLanguage.h"
#include "InvalidDataLength.h"
#include "InvalidSession.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddEvent("P64-16:P64-16"));
    add(new DomainEventLog("P64-4:P64-5"));
    add(new NullEvtEntry("P64-18:P64-18"));
    add(new InvalidEventType("P64-19:P64-20"));
    add(new InvalidSourceField("P64-19:P64-20"));
    add(new InvalidSeverity("P64-21:P64-21"));
    add(new InvalidDataType("P64-24:P64-24"));
    add(new InvalidDataField("P64-25:P64-25"));
    add(new InvalidLanguage("P64-26:P64-27"));
    add(new InvalidDataLength("P64-28:P64-29"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiEventAdd";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiEventAdd().";
}

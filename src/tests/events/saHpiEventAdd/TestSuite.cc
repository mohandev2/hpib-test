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

  add(new AddEvent("B.02|P80-16:P80-16"));
    add(new DomainEventLog("B.02|P80-4:P80-5"));
    add(new NullEvtEntry("B.02|P80-18:P80-18"));
    add(new InvalidEventType("B.02|P80-19:P80-20"));
    add(new InvalidSourceField("B.02|P80-19:P80-20"));
    add(new InvalidSeverity("B.02|P80-21:P80-21"));
    add(new InvalidDataType("B.02|P80-24:P80-24"));
    add(new InvalidDataField("B.02|P80-25:P80-25"));
    add(new InvalidLanguage("B.02|P80-26:P80-27"));
    add(new InvalidDataLength("B.02|P80-28:P80-29"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

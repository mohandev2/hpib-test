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
#include "SetState.h"
#include "NoCapability.h"
#include "MissingCapability.h"
#include "AddEntryDisabled.h"
#include "AddEntryEnabled.h"
#include "GenEventDisabled.h"
#include "GenEventEnabled.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiEventLogStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    /*add(new SetState("P57-16:P57-16"));
    add(new NoCapability("P57-17:P57-18"));
    add(new AddEntryDisabled("P57-21:P57-22"));
    add(new AddEntryEnabled("P57-21:P57-22"));
    add(new GenEventDisabled("P57-22:P57-23"));
    add(new GenEventEnabled("P57-22:P57-23"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
 */

    add(new SetState("B.02|P73-16:P73-16"));
    add(new NoCapability("B.02|P73-17:P73-18"));
    add(new MissingCapability("B.02|P73-17:P73-18"));
    add(new AddEntryDisabled("B.02|P73-21:P73-22"));
    add(new AddEntryEnabled("B.02|P73-21:P73-22"));
    add(new GenEventDisabled("B.02|P73-22:P73-23"));
    add(new GenEventEnabled("B.02|P73-22:P73-23"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiEventLogStateSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiEventLogStateSet().";
}

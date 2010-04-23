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
#include "SameState.h"
#include "NoCapability.h"
#include "PowerCycleOn.h"
#include "PowerCycleOff.h"
#include "PowerToggle.h"
#include "InvalidState.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiResourcePowerStateSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SameState("P157-5:P157-5"));
    add(new PowerToggle("P159-15:P159-15"));
    add(new PowerCycleOn("P159-17:P159-17"));
    add(new PowerCycleOff("P157-7:P157-8"));
    add(new NoCapability("P159-20:P159-21"));
    add(new InvalidState("P159-22:P159-22"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiResourcePowerStateSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiResourcePowerStateSet().";
}

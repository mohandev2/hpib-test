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
 
    add(new SameState("B.02|P215-5:P215-5"));
    add(new PowerToggle("B.02|P217-15:P217-15"));
    add(new PowerCycleOn("B.02|P217-17:P217-17"));
    add(new PowerCycleOff("B.02|P215-7:P215-8"));
    add(new NoCapability("B.02|P217-20:P217-21"));
    add(new InvalidState("B.02|P217-22:P217-22"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

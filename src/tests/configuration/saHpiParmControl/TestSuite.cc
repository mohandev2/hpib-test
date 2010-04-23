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
#include "AutoExtractTimeout.h"
#include "SensorEnableState.h"
#include "SensorEventEnableState.h"
#include "SensorEventMasks.h"
#include "SensorThresholds.h"
#include "NoCapability.h"
#include "InvalidAction.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiParmControl;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new AutoExtractTimeout("B.02|P208-22:P208-22;P207-5:P207-5"));
    add(new SensorEnableState("B.02|P208-22:P208-22;P207-6:P207-6"));
    add(new SensorEventEnableState("B.02|P208-22:P208-22;P207-7:P207-7"));
    add(new SensorEventMasks("B.02|P208-22:P208-22;P207-8:P207-8"));
    add(new SensorThresholds("B.02|P208-22:P208-22;P207-9:P207-9"));
    add(new NoCapability("B.02|P208-23:P208-24"));
    add(new InvalidAction("B.02|P208-25:P208-25"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiParmControl";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiParmControl().";
}

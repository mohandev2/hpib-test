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
#include "SetUpCritical.h"
#include "MultiSet.h"
#include "InOrder.h"
#include "InvalidFormat.h"
#include "NoCapability.h"
#include "NonThresholdSensor.h"
#include "SensorNotPresent.h"
#include "NonWritableThreshold.h"
#include "MinRange.h"
#include "MaxRange.h"
#include "OutOfOrder.h"
#include "NegativeHysteresis.h"
#include "Reject.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiSensorThresholdsSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SetUpCritical("P82-16:P82-16"));
    add(new MultiSet("P82-16:P82-16"));
    add(new InOrder("P82-16:P82-16"));
    add(new InvalidFormat("P82-17:P82-18"));
    add(new NoCapability("P82-19:P82-20"));
    add(new SensorNotPresent("P82-21:P82-21"));
    add(new NonWritableThreshold("P82-23:P82-23"));
    add(new NonThresholdSensor("P82-24:P82-25"));
    add(new MinRange("P82-26:P82-27"));
    add(new MaxRange("P82-26:P82-27"));
    add(new OutOfOrder("P82-29:P82-29"));
    add(new NegativeHysteresis("P82-30:P82-30"));
    add(new Reject());
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSensorThresholdsSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSensorThresholdsSet().";
}

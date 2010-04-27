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
#include "NullSensorThresholds.h"
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

    add(new SetUpCritical("B.02|P100-16:P100-16"));
    add(new MultiSet("B.02|P100-16:P100-16"));
    add(new InOrder("B.02|P100-16:P100-16"));
    add(new InvalidFormat("B.02|P100-17:P100-18"));
    add(new NoCapability("B.02|P100-19:P100-20"));
    add(new SensorNotPresent("B.02|P100-21:P100-21"));
    add(new NonWritableThreshold("B.02|P100-23:P100-23"));
    add(new NonThresholdSensor("B.02|P100-24:P100-25"));
    add(new NullSensorThresholds("B.02|P100-24:P100-25"));
    add(new MinRange("B.02|P100-26:P100-27"));
    add(new MaxRange("B.02|P100-26:P100-27"));
    add(new OutOfOrder("B.02|P100-29:P100-29"));
    add(new NegativeHysteresis("B.02|P100-30:P100-30"));
    add(new Reject());
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

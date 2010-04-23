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
#include "GetThresholds.h"
#include "NoCapability.h"
#include "NullSensorThresholds.h"
#include "NonThresholdSensor.h"
#include "NoReadableThresholds.h"
#include "SensorNotPresent.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiSensorThresholdsGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetThresholds("P81-16:P81-16"));
    add(new NoCapability("P81-17:P81-18"));
    add(new NullSensorThresholds("P81-19:P81-19"));
    add(new NonThresholdSensor("P81-21:P81-21"));
    add(new NoReadableThresholds("P81-22:P81-22"));
    add(new SensorNotPresent("P81-23:P81-23"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSensorThresholdsGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSensorThresholdsGet().";
}

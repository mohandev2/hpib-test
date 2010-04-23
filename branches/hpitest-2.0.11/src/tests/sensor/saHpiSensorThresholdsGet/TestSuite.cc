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
    add(new GetThresholds("B.02|P99-16:P99-16"));
    add(new NoCapability("B.02|P99-17:P99-18"));
    add(new NullSensorThresholds("B.02|P99-19:P99-19"));
    add(new NonThresholdSensor("B.02|P99-21:P99-21"));
    add(new NoReadableThresholds("B.02|P99-22:P99-22"));
    add(new SensorNotPresent("B.02|P99-23:P99-23"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

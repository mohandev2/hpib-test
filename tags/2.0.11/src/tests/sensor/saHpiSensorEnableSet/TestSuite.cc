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
#include "SetEnableStatus.h"
#include "NoCapability.h"
#include "SensorNotPresent.h"
#include "ReadOnly.h"
#include "DisabledSensor.h"
#include "SensorEnableChangeEvent.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiSensorEnableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new SetEnableStatus("B.02|P104-16:P104-16"));
    add(new NoCapability("B.02|P104-17:P104-18"));
    add(new SensorNotPresent("B.02|P104-19:P104-19"));
    add(new ReadOnly("B.02|P104-20:P104-21"));
    add(new DisabledSensor("B.02|P104-23:P104-24"));
    add(new SensorEnableChangeEvent("B.02|P104-27:P104-27"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSensorEnableSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSensorEnableSet().";
}

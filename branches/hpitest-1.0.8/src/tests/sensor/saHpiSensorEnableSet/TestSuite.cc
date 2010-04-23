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
    add(new SetEnableStatus("P86-16:P86-16"));
    add(new NoCapability("P86-17:P86-18"));
    add(new SensorNotPresent("P86-19:P86-19"));
    add(new ReadOnly("P86-20:P86-21"));
    add(new DisabledSensor("P86-23:P86-24"));
    add(new SensorEnableChangeEvent("P86-27:P86-27"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
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

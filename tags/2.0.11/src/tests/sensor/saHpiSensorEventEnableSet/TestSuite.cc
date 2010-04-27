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
#include "SetEventEnableStatus.h"
#include "NoCapability.h"
#include "SensorNotPresent.h"
#include "ReadOnly.h"
#include "DisabledEvents.h"
#include "ReadEventStates.h"
#include "SensorEnableChangeEvent.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiSensorEventEnableSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SetEventEnableStatus("P88-16:P88-16"));
    add(new NoCapability("P88-17:P88-18"));
    add(new SensorNotPresent("P88-19:P88-19"));
    add(new ReadOnly("P88-20:P88-21"));
    add(new DisabledEvents("P88-23:P88-24"));
    add(new ReadEventStates("P88-26:P88-27"));
    add(new SensorEnableChangeEvent("P88-31:P88-31"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSensorEventEnableSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSensorEventEnableSet().";
}

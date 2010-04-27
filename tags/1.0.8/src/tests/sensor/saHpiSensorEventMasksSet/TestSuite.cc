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
#include "SetMasks.h"
#include "NoCapability.h"
#include "UnsupportedEventState.h"
#include "InvalidAction.h"
#include "SensorNotPresent.h"
#include "ReadOnly.h"
#include "RemoveAllStates.h"
#include "AddAllStates.h"
#include "SensorEnableChangeEventAssert.h"
#include "SensorEnableChangeEvent.h"
#include "EvtDeasserts.h"
#include "NotEvtDeasserts.h"
#include "NoUpMinorEvent.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiSensorEventMasksSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SetMasks("P90-30:P90-30"));
    add(new NoCapability("P90-31:P90-32"));
    add(new UnsupportedEventState("P90-33:P90-35"));
    add(new InvalidAction("P90-36:P90-36"));
    add(new SensorNotPresent("P90-37:P90-37"));
    add(new ReadOnly("P90-38:P90-39"));
    add(new NoUpMinorEvent("P91-5:P91-7"));
    add(new EvtDeasserts("P91-9:P91-13"));
    add(new NotEvtDeasserts("P91-9:P91-10"));
    add(new RemoveAllStates("P91-14:P90-16"));
    add(new AddAllStates("P91-14:P91-16"));
    add(new SensorEnableChangeEventAssert("P91-17:P91-18"));
    add(new SensorEnableChangeEvent("P91-17:P91-18"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSensorEventMasksSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSensorEventMasksSet().";
}

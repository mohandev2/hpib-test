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

    add(new SetMasks("B.02|P108-30:P108-30"));
    add(new NoCapability("B.02|P108-31:P108-32"));
    add(new UnsupportedEventState("B.02|P108-33:P108-35"));
    add(new InvalidAction("B.02|P108-36:P108-36"));
    add(new SensorNotPresent("B.02|P108-37:P108-37"));
    add(new ReadOnly("B.02|P108-38:P108-39"));
    add(new NoUpMinorEvent("B.02|P109-5:P109-7"));
    add(new EvtDeasserts("B.02|P109-9:P109-13"));
    add(new NotEvtDeasserts("B.02|P109-9:P109-10"));
    add(new RemoveAllStates("B.02|P109-14:P108-16"));
    add(new AddAllStates("B.02|P109-14:P109-16"));
    add(new SensorEnableChangeEventAssert("B.02|P109-17:P109-18"));
    add(new SensorEnableChangeEvent("B.02|P109-17:P109-18"));
    add(new InvalidResource("B.02|P29-40:P29-42"));
    add(new InvalidSession("B.02|P29-43:P29-45"));

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

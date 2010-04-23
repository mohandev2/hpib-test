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
    add(new AutoExtractTimeout("P153-22:P153-22;P152-5:P152-5"));
    add(new SensorEnableState("P153-22:P153-22;P152-6:P152-6"));
    add(new SensorEventEnableState("P153-22:P153-22;P152-7:P152-7"));
    add(new SensorEventMasks("P153-22:P153-22;P152-8:P152-8"));
    add(new SensorThresholds("P153-22:P153-22;P152-9:P152-9"));
    add(new NoCapability("P153-23:P153-24"));
    add(new InvalidAction("P153-25:P153-25"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
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

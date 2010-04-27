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
#include "GetAndVerify.h"
#include "NoCapability.h"
#include "DisabledSensor.h"
#include "SensorNotPresent.h"
#include "NullParams.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiSensorReadingGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetAndVerify("P80-20:P80-20;P80-28:P80-30;P80-31:P80-34"));
    add(new NoCapability("P80-21:P80-22"));
    add(new DisabledSensor("P80-23:P80-23"));
    add(new SensorNotPresent("P80-24:P80-24"));
    add(new NullParams("P80-15:P80-18;P80-35:P80-38"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSensorReadingGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSensorReadingGet().";

}

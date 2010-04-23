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
  
    add(new GetAndVerify("B.02|P98-20:P98-20;P98-28:P98-30;P98-31:P98-34"));
    add(new NoCapability("B.02|P98-21:P98-22"));
    add(new DisabledSensor("B.02|P98-23:P98-23"));
    add(new SensorNotPresent("B.02|P98-24:P98-24"));
    add(new NullParams("B.02|P98-15:P98-18;P98-35:P98-38"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

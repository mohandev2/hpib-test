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
#include "GetState.h"
#include "NoCapability.h"
#include "NullState.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiResourcePowerStateGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new GetState("B.02|P216-18:P216-18"));
    add(new NoCapability("B.02|P216-19:P216-20"));
    add(new NullState("B.02|P216-21:P216-21"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiResourcePowerStateGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiResourcePowerStateGet().";
}

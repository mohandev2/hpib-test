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
 *     Meghana Srivatsavai <mjn77@unh.edu>
 */

#include "TestSuite.h"
#include "GetInfo.h"
#include "NoCapability.h"
#include "NullInfo.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiFumiSpecInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetInfo("B.03|P190-18:P62-18"));
    add(new NoCapability("B.03|P190-19:P62-20"));
    add(new NullInfo("B.03|P190-22:P62-22"));
    add(new InvalidResource("B.03|P190-15:P36-16"));
    add(new InvalidSession("B.03|P190-17:P36-18"));
}


/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiFumiSpecInfoGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiFumiSpecInfoGet().";
}

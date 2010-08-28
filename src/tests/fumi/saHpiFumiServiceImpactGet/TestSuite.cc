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
#include "GetServiceImpact.h"
#include "NoCapability.h"
#include "NullServiceImpact.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiFumiServiceImpactGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetServiceImpact("B.03|P191-18:P191-18"));
    add(new NoCapability("B.03|P191-19:P191-20"));
    add(new NullServiceImpact("B.03|P191-22:P191-22"));
    add(new InvalidResource("B.03|P191-15:P36-16"));
    add(new InvalidSession("B.03|P191-17:P36-18"));
}


/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiFumiServiceImpactGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiFumiServiceImpactGet().";
}

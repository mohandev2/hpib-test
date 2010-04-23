/*      
 * (C) Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Suntrupth S Yadav <suntrupth@in.ibm.com>
 *
 *  Modified by:
 *  Akmal Muqeeth <aax6@unh.edu>
 *  Pushpa Datla  <pgr4@unh.edu>
 */

#include "TestSuite.h"
#include "GetCapabilities.h"
#include "NoCapability.h"
#include "NullCapabilities.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiEventLogCapabilitiesGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new GetCapabilities("B.02.01| P63-18:P63-18"));
    add(new NoCapability("B.02.01| P63-19:P63-20"));
    add(new NullCapabilities("B.02.01| P63-22:P63-22"));
    add(new InvalidResource("B.02.01| P36-40:P36-42"));
    add(new InvalidSession("B.02.01| P36-43:P36-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiEventLogCapabilitiesGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiEventLogCapabilitiesGet().";
}

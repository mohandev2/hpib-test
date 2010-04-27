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
*  Akmal Muqeeth <aax6@unh.edu>
*

*/

#include "TestSuite.h"
#include "InvalidSession.h"
#include "ResourceIdNotPresent.h"
#include "NoFruCapability.h"
#include "FailedRemove.h"
#include "InvalidRequest.h"


using namespace ns_saHpiResourceFailedRemove;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    
    add(new InvalidSession("B.02 |P36-43:P36-45"));
    add(new ResourceIdNotPresent("B.02 | P59-21:P59-21"));
    add(new NoFruCapability("B.02 | P59-23:P59-24"));
    add(new FailedRemove("B.02 | P59-23:P59-24"));
    add(new InvalidRequest("B.02 | P59-23:P59-24"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiResourceFailedRemove";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiResourceFailedRemove().";
}

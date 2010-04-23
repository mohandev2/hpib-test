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
*

*/

#include "TestSuite.h"
#include "NullResourceId.h"
#include "InvalidSession.h"
#include "NullInstanceId.h"
#include "NullRptUpdateCount.h"
#include "ReservedValueIns.h"
#include "NullInstrumentId.h"
#include "ResInstNotPresent.h"
#include "GetResInst.h"

using namespace ns_saHpiGetIdByEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    
    add(new NullResourceId("B.02 |P55-14:P55-14"));
    add(new InvalidSession("B.02 |P36-43:P36-45"));
    add(new NullInstanceId("B.02 |P55-16:P55-16"));
    add(new NullRptUpdateCount("B.02 |P55-4:P55-8"));
    add(new ReservedValueIns("B.02 |P55-26:P55-26"));
    add(new NullInstrumentId("B.02| P76-27:P76-28"));
    add(new ResInstNotPresent("B.02| P76-27:P76-28"));
    add(new GetResInst("B.02| P76-27:P76-28"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiGetIdByEntityPath";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiGetIdByEntityPath().";
}


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
#include "NullInstanceId.h"
#include "InvalidSession.h"
#include "NullRptUpdateCount.h"
#include "ReservedValueIns.h"
#include "InvalidParentEntityPath.h"
#include "FirstEntryInstanceId.h"
#include "OutOfRangeInstanceId.h"
#include "GetChildEntityPath.h"

using namespace ns_saHpiGetChildEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    
    add(new FirstEntryInstanceId("B.02.01 |P57-4:P57-8"));
    add(new InvalidParentEntityPath("B.02.01 | P57-43:P57-45"));
    add(new InvalidSession("B.02.01 | P57-43:P57-45"));
    add(new NullInstanceId("B.02.01 | P57-16:P57-16"));
    add(new NullRptUpdateCount("B.02.01 |P57-4:P57-8"));
    add(new ReservedValueIns("B.02.01 | P57-26:P57-26"));
    add(new OutOfRangeInstanceId("B.02.01 | P57-26:P57-26"));
    add(new GetChildEntityPath("B.02.01 | P57-26:P57-26"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiGetChildEntityPath";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiGetChildEntityPath().";
}

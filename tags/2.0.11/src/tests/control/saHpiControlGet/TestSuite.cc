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
#include "GetControl.h"
#include "WriteOnly.h"
#include "InvalidLineNumber.h"
#include "ControlNotPresent.h"
#include "NullParams.h"
#include "NoCapability.h"
#include "AllLines.h"
#include "MaxCharsLines.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiControlGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new GetControl("B.02|P112-20:P112-20"));
    add(new WriteOnly("B.02|P112-21:P112-22"));
    add(new NoCapability("B.02|P112-23:P112-24"));
    add(new InvalidLineNumber("B.02|P112-25:P112-26"));
    add(new ControlNotPresent("B.02|P112-27:P112-27"));
    add(new NullParams("B.02|P112-15:P112-18"));
    add(new AllLines("B.02|P113-1:P113-4"));
    add(new MaxCharsLines("B.02|P113-7:P113-8"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiControlGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiControlGet().";
}

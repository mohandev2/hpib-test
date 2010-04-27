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
    add(new GetControl("P94-20:P94-20"));
    add(new WriteOnly("P94-21:P94-22"));
    add(new NoCapability("P94-23:P94-24"));
    add(new InvalidLineNumber("P94-25:P94-26"));
    add(new ControlNotPresent("P94-27:P94-27"));
    add(new NullParams("P94-15:P94-18"));
    add(new AllLines("P95-1:P95-4"));
    add(new MaxCharsLines("P95-7:P95-8"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
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

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
#include "GetRdr.h"
#include "NoRdrCapability.h"
#include "NoInstrumentCapability.h"
#include "InvalidInstrumentId.h"
#include "InvalidRdrType.h"
#include "NoRecordRdrType.h"
#include "NullRdr.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiRdrGetByInstrumentId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetRdr("P76-21:P76-21"));
    add(new NoRdrCapability("P76-23:P76-24"));
    add(new NoInstrumentCapability("P76-25:P76-26"));
    add(new InvalidInstrumentId("P76-27:P76-28"));
    add(new InvalidRdrType("P76-30:P76-30"));
    add(new NoRecordRdrType("P76-31:P76-31"));
    add(new NullRdr("P76-32:P76-32"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiRdrGetByInstrumentId";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiRdrGetByInstrumentId().";
}

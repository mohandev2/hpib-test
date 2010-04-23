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

    add(new GetRdr("B.02|P93-21:P93-21"));
    add(new NoRdrCapability("B.02|P93-23:P93-24"));
    add(new NoInstrumentCapability("B.02|P93-25:P93-26"));
    add(new InvalidInstrumentId("B.02|P93-27:P93-28"));
    add(new InvalidRdrType("B.02|P93-30:P93-30"));
    add(new NoRecordRdrType("B.02|P93-31:P93-31"));
    add(new NullRdr("B.02|P93-32:P93-32"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

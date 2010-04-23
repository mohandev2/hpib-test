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
#include "GetAll.h"
#include "NoCapability.h"
#include "InvalidEntryId.h"
#include "LastEntry.h"
#include "NullNextEntryId.h"
#include "NullRdr.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiRdrGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new GetAll("B.02|P92-20:P92-20"));
    add(new NoCapability("B.02|P92-21:P92-22"));
    add(new InvalidEntryId("B.02|P92-23:P92-24"));
    add(new LastEntry("B.02|P92-26:P92-26"));
    add(new NullNextEntryId("B.02|P92-27:P92-27"));
    add(new NullRdr("B.02|P92-28:P92-28"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiRdrGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiRdrGet().";
}

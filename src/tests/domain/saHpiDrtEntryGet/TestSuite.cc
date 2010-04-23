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
#include "InvalidSession.h"
#include "InvalidEntryId.h"
#include "NullDrtEntry.h"
#include "NullNextEntryId.h"
#include "ReservedEntryId.h"

using namespace ns_saHpiDrtEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new GetAll("B.02|P44-19:P44-19"));
    add(new InvalidEntryId("B.02|P44-20:P44-21"));
    add(new NullDrtEntry("B.02|P44-24:P44-24"));
    add(new NullNextEntryId("B.02|P44-25:P44-25"));
    add(new ReservedEntryId("B.02|P44-26:P44-26"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiDrtEntryGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiDrtEntryGet().";
}

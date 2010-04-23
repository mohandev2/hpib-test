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
#include "NullRptEntry.h"
#include "NullNextEntryId.h"
#include "InvalidSession.h"
#include "EntryIdNotPresent.h"
#include "ReservedValue.h"
#include "AutoUpdate.h"

using namespace ns_saHpiRptEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetAll("P40-19:P40-19"));
    add(new EntryIdNotPresent("P40-21:P40-21"));
    add(new NullRptEntry("P40-24:P40-24"));
    add(new NullNextEntryId("P40-25:P40-25"));
    add(new ReservedValue("P40-26:P40-26"));
    add(new AutoUpdate("P41-4:P41-8"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiRptEntryGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiRptEntryGet().";
}

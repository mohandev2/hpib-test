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
#include "ForwardTraversal.h"
#include "BackwardTraversal.h"
#include "NoCapability.h"
#include "EmptyEventLog.h"
#include "EntryIdNotPresent.h"
#include "NullPointers.h"
#include "NoMoreEntries.h"
#include "Selection.h"
#include "NoRptEntry.h"
#include "NoRdr.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiEventLogEntryGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new ForwardTraversal("P49-39:P49-39, P50-16:P50-18"));
    add(new BackwardTraversal("P49-39:P49-39, P50-19:P50-21"));
    add(new NoCapability("P50-1:P50-2"));
    add(new EmptyEventLog("P50-5:P50-5"));
    add(new EntryIdNotPresent("P50-6:P50-6"));
    add(new NullPointers("P50-8:P50-8"));
    add(new NoMoreEntries("P50-9:P50-9"));
    add(new Selection("P50-11:P50-12"));
    add(new NoRptEntry("P50-27:P50-29"));
    add(new NoRdr("P50-30:P50-31"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiEventLogEntryGet";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiEventLogEntryGet().";
}

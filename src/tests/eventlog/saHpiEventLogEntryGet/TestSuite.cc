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

    add(new ForwardTraversal("B.02|P64-39:P64-39, P65-16:P65-18"));
    add(new BackwardTraversal("B.02|P64-39:P64-39, P65-19:P65-21"));
    add(new NoCapability("B.02|P65-1:P65-2"));
    add(new EmptyEventLog("B.02|P65-5:P65-5"));
    add(new EntryIdNotPresent("B.02|P65-6:P65-6"));
    add(new NullPointers("B.02|P65-8:P65-8"));
    add(new NoMoreEntries("B.02|P65-9:P65-9"));
    add(new Selection("B.02|P65-11:P65-12"));
    add(new NoRptEntry("B.02|P65-27:P65-29"));
    add(new NoRdr("B.02|P65-30:P65-31"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

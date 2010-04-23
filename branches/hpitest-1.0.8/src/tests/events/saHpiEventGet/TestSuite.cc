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
#include "NoSubscription.h"
#include "NullEvent.h"
#include "NegativeTimeout.h"
#include "Overflow.h"
#include "OverflowReset.h"
#include "ResourceCapabilities.h"
#include "NoRecord.h"
#include "CloseSession.h"
#include "Unsubscribe.h"
#include "RptEntryAndRdr.h"
#include "TimeoutNormal.h"
#include "TimeoutImmediate.h"
#include "EventBeforeNormal.h"
#include "EventBeforeImmediate.h"
#include "EventBeforeBlock.h"
#include "EventDuringNormal.h"
#include "EventDuringBlock.h"
#include "InvalidSession.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new RptEntryAndRdr("P62-20:P62-24:P63-10:P63-10"));
    add(new NoSubscription("P62-28:P62-28"));
    add(new NullEvent("P62-30:P62-30"));
    add(new NegativeTimeout("P62-31:P62-31"));
    add(new Overflow("P62-36:P62-39"));

    add(new EventBeforeNormal("P63-1:P63-2"));
    add(new EventBeforeImmediate("P63-1:P63-2"));
    add(new EventBeforeBlock("P63-1:P63-2"));

    add(new TimeoutNormal("P63-2:P63-3"));
    add(new TimeoutImmediate("P63-3:P63-5"));

    add(new EventDuringNormal("P63-3:P63-4"));
    add(new EventDuringBlock("P63-5:P63-6"));

    add(new ResourceCapabilities("P63-8:P63-10"));
    add(new NoRecord("P63-11:P63-13"));

    add(new OverflowReset("P63-22:P63-23"));
    add(new CloseSession("P63-26:P63-27"));
    add(new Unsubscribe("P63-27:P63-30"));

    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiEventGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiEventGet().";
}

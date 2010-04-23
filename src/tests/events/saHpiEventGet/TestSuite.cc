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

    add(new RptEntryAndRdr("B.02|P78-20:P78-24:P79-10:P79-10"));
    add(new NoSubscription("B.02|P78-28:P78-28"));
    add(new NullEvent("B.02|P78-30:P78-30"));
    add(new NegativeTimeout("B.02|P78-31:P78-31"));
    add(new Overflow("B.02|P78-36:P78-39"));

    add(new EventBeforeNormal("B.02|P79-1:P79-2"));
    add(new EventBeforeImmediate("B.02|P79-1:P79-2"));
    add(new EventBeforeBlock("B.02|P79-1:P79-2"));

    add(new TimeoutNormal("B.02|P79-2:P79-3"));
    add(new TimeoutImmediate("B.02|P79-3:P79-5"));

    add(new EventDuringNormal("B.02|P79-3:P79-4"));
    add(new EventDuringBlock("B.02|P79-5:P79-6"));

    add(new ResourceCapabilities("B.02|P79-8:P79-10"));
    add(new NoRecord("B.02|P79-11:P79-13"));

    add(new OverflowReset("B.02|P79-22:P79-23"));
    add(new CloseSession("B.02|P79-26:P79-27"));
    add(new Unsubscribe("B.02|P79-27:P79-30"));

    add(new InvalidSession("B.02|P36-17:P36-18"));

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

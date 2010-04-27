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
#include "saHpiSubscribe/TestSuite.h"
#include "saHpiUnsubscribe/TestSuite.h"
#include "saHpiEventGet/TestSuite.h"
#include "saHpiEventAdd/TestSuite.h"

using namespace ns_events;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() { 
    add(new ns_saHpiSubscribe::TestSuite());
    add(new ns_saHpiUnsubscribe::TestSuite());
    add(new ns_saHpiEventGet::TestSuite());
    add(new ns_saHpiEventAdd::TestSuite());
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "Events";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Events.";
}

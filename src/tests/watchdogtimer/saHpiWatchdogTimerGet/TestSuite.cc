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
#include "GetTimer.h"
#include "Countdown.h"
#include "NoCapability.h"
#include "WatchdogNotPresent.h"
#include "NullWatchdog.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiWatchdogTimerGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetTimer("P116-16:P116-16"));
    add(new Countdown("P181-30:P181-31;P181-64:P181-65"));
    add(new NoCapability("P116-17:P116-18"));
    add(new WatchdogNotPresent("P116-19:P116-19"));
    add(new NullWatchdog("P116-20:P116-20"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiWatchdogTimerGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiWatchdogTimerGet().";
}

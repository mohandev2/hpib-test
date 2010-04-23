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
    add(new GetTimer("B.02|P140-16:P140-16"));
    add(new Countdown("B.02|P242-30:P242-31;P242-64:P242-65"));
    add(new NoCapability("B.02|P140-17:P140-18"));
    add(new WatchdogNotPresent("B.02|P140-19:P140-19"));
    add(new NullWatchdog("B.02|P140-20:P140-20"));
    add(new InvalidResource("B.02|P29-15:P29-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

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
#include "SetTimer.h"
#include "NoCapability.h"
#include "WatchdogNotPresent.h"
#include "NullWatchdog.h"
#include "Running.h"
#include "Restart.h"
#include "InvalidTimerUse.h"
#include "InvalidTimerAction.h"
#include "InvalidPretimerInterrupt.h"
#include "InvalidPreTimeoutInterval.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiWatchdogTimerSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
/*    add(new SetTimer("P117-20:P117-20"));
    add(new NoCapability("P117-21:P117-22"));
    add(new WatchdogNotPresent("P117-23:P117-23"));
    add(new InvalidTimerUse("P117-25:P117-25"));
    add(new InvalidTimerAction("P117-26:P117-26"));
    add(new InvalidPretimerInterrupt("P117-27:P117-27"));
    add(new InvalidPreTimeoutInterval("P117-32:P117-32"));
    add(new NullWatchdog("P117-28:P117-28"));
    add(new Running("P118-3:P118-6"));
    add(new Restart("P118-6:P118-7"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
 */ 

    add(new SetTimer("B.02|P141-20:P141-20"));
    add(new NoCapability("B.02|P141-21:P141-22"));
    add(new WatchdogNotPresent("B.02|P141-23:P141-23"));
    add(new InvalidTimerUse("B.02|P141-25:P141-25"));
    add(new InvalidTimerAction("B.02|P141-26:P141-26"));
    add(new InvalidPretimerInterrupt("B.02|P141-27:P141-27"));
    add(new InvalidPreTimeoutInterval("B.02|P141-32:P141-32"));
    add(new NullWatchdog("B.02|P141-28:P141-28"));
    add(new Running("B.02|P142-3:P142-6"));
    add(new Restart("B.02|P142-6:P142-7"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));


}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiWatchdogTimerSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiWatchdogTimerSet().";
}

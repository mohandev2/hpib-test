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
#include "Event.h"
#include "PretimerEvent.h"
#include "ExpirationFlags.h"

using namespace ns_saHpiWatchdogFunctional;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new Event("B.02|P140-16:P140-18"));
    add(new PretimerEvent("B.02|P241-11:P241-12"));
    add(new ExpirationFlags("B.02|P241-50:P241-54;P242-36:P242-38"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "functional";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Watchdog Functional Operations.";
}

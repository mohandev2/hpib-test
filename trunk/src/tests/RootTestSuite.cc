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
 *
 * Changes:
 * 09/11/27 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Add Fumi support
 */

#include "RootTestSuite.h"
#include "version/TestSuite.h"
#include "session/TestSuite.h"
#include "domain/TestSuite.h"
#include "resource/TestSuite.h"
#include "eventlog/TestSuite.h"
#include "events/TestSuite.h"
#include "alarm/TestSuite.h"
#include "rdr/TestSuite.h"
#include "sensor/TestSuite.h"
#include "control/TestSuite.h"
#include "inventory/TestSuite.h"
#include "watchdogtimer/TestSuite.h"
#include "annunciator/TestSuite.h"
#include "hotswap/TestSuite.h"
#include "configuration/TestSuite.h"
#include "reset/TestSuite.h"
#include "power/TestSuite.h"
#include "fumi/TestSuite.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

RootTestSuite::RootTestSuite() {
    add(new ns_version::TestSuite());
    add(new ns_session::TestSuite());
    add(new ns_domain::TestSuite());
    add(new ns_resource::TestSuite());
    add(new ns_eventlog::TestSuite());
    add(new ns_events::TestSuite());
    add(new ns_alarm::TestSuite());
    add(new ns_rdr::TestSuite());
    add(new ns_sensor::TestSuite());
    add(new ns_control::TestSuite());
    add(new ns_inventory::TestSuite());
    add(new ns_watchdogtimer::TestSuite());
    add(new ns_annunciator::TestSuite());
    add(new ns_hotswap::TestSuite());
    add(new ns_configuration::TestSuite());
    add(new ns_reset::TestSuite());
    add(new ns_power::TestSuite());
    add(new ns_fumi::TestSuite());
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *RootTestSuite::getName() {
    return NULL;
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *RootTestSuite::getDescription() {
    return "HPI Conformance test suite.";
}


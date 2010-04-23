/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 */

#include "TestSuite.h"
#include "NoCapability.h"
#include "NullCounter.h"
#include "InvalidSession.h"
#include "InvalidResource.h"
#include "GetUpdateCount.h"

using namespace ns_saHpiRdrUpdateCountGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new NoCapability("B.03|P109-15:P109-16"));
    add(new InvalidResource("B.03|P44-13:P44-15"));
    add(new InvalidSession("B.03|P44-10:P44-10"));
    add(new NullCounter("B.03|P109-18:P109-18"));
    add(new GetUpdateCount("B.03|P109-14:P109-14"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiRdrUpdateCountGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiRdrUpdateCountGet().";
}

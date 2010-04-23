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
#include "PolicyCancel.h"
#include "NoCapability.h"
#include "NotPending.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiHotSwapPolicyCancel;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new PolicyCancel("P138-17:P138-17"));
    add(new NoCapability("P138-18:P138-19"));
    add(new NotPending("P138-21:P138-21"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiHotSwapPolicyCancel";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiHotSwapPolicyCancel().";
}

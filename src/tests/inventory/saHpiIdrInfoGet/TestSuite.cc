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
#include "GetInfo.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "NullIdrInfo.h"
#include "UpdateCounter.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetInfo("B.02|P121-17:P101-17"));
    add(new NoCapability("B.02|P121-18:P101-19"));
    add(new IdrNotPresent("B.02|P121-20:P101-20"));
    add(new NullIdrInfo("B.02|P121-21:P101-21"));
    add(new UpdateCounter("B.02|P121-23:P101-28"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrInfoGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrInfoGet().";
}

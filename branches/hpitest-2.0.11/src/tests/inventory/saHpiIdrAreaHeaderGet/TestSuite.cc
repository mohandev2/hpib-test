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
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaNotPresent.h"
#include "AreaTypeMismatch.h"
#include "InvalidAreaType.h"
#include "ReservedAreaId.h"
#include "NullNextAreaId.h"
#include "NullHeader.h"
#include "Traversal.h"
#include "TraversalByAreaType.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrAreaHeaderGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new NoCapability("B.02|P122-26:P122-27"));
    add(new IdrNotPresent("B.02|P122-29:P122-29"));
    add(new AreaNotPresent("B.02|P122-30:P122-31"));
    add(new AreaTypeMismatch("B.02|P122-32:P122-33"));
    add(new InvalidAreaType("B.02|P122-35:P122-35"));
    add(new ReservedAreaId("B.02|P122-36:P122-36"));
    add(new NullNextAreaId("B.02|P122-37:P122-37"));
    add(new NullHeader("B.02|P122-38:P122-38"));
    add(new Traversal("B.02|P123-4:P123-7"));
    add(new TraversalByAreaType("B.02|P123-8:P123-11"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrAreaHeaderGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrAreaHeaderGet().";
}

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
    add(new NoCapability("P102-26:P102-27"));
    add(new IdrNotPresent("P102-29:P102-29"));
    add(new AreaNotPresent("P102-30:P102-31"));
    add(new AreaTypeMismatch("P102-32:P102-33"));
    add(new InvalidAreaType("P102-35:P102-35"));
    add(new ReservedAreaId("P102-36:P102-36"));
    add(new NullNextAreaId("P102-37:P102-37"));
    add(new NullHeader("P102-38:P102-38"));
    add(new Traversal("P103-4:P103-7"));
    add(new TraversalByAreaType("P103-8:P103-11"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
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

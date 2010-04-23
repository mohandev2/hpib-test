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
#include "FieldNotPresent.h"
#include "FieldTypeMismatch.h"
#include "InvalidFieldType.h"
#include "ReservedAreaId.h"
#include "ReservedFieldId.h"
#include "NullNextFieldId.h"
#include "NullField.h"
#include "Traversal.h"
#include "TraversalByFieldType.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrFieldGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new NoCapability("P107-28:P107-29"));
    add(new IdrNotPresent("P107-31:P107-31"));
    add(new AreaNotPresent("P107-32:P107-32"));
    add(new FieldNotPresent("P107-33:P107-34"));
    add(new FieldTypeMismatch("P107-35:P107-36"));
    add(new InvalidFieldType("P107-38:P107-38"));
    add(new ReservedAreaId("P108-1:P108-1"));
    add(new ReservedFieldId("P108-1:P108-1"));
    add(new NullNextFieldId("P108-2:P108-2"));
    add(new NullField("P108-3:P108-3"));
    add(new Traversal("P108-7:P108-10"));
    add(new TraversalByFieldType("P108-11:P108-14"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrFieldGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrFieldGet().";
}

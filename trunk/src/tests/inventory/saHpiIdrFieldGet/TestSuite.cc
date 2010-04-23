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

    add(new NoCapability("B.02|P129-28:P129-29"));
    add(new IdrNotPresent("B.02|P129-31:P129-31"));
    add(new AreaNotPresent("B.02|P129-32:P129-32"));
    add(new FieldNotPresent("B.02|P129-33:P129-34"));
    add(new FieldTypeMismatch("B.02|P129-35:P129-36"));
    add(new InvalidFieldType("B.02|P129-38:P129-38"));
    add(new ReservedAreaId("B.02|P130-1:P130-1"));
    add(new ReservedFieldId("B.02|P130-1:P130-1"));
    add(new NullNextFieldId("B.02|P130-2:P130-2"));
    add(new NullField("B.02|P130-3:P130-3"));
    add(new Traversal("B.02|P130-7:P130-10"));
    add(new TraversalByFieldType("B.02|P130-11:P130-14"));
    add(new InvalidResource("B.02|P30-15:P30-16"));
    add(new InvalidSession("B.02|P30-17:P30-18"));

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

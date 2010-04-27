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
#include "DeleteField.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaNotPresent.h"
#include "FieldNotPresent.h"
#include "ReadOnlyField.h"
#include "ReadOnlyArea.h"
#include "ReadOnlyIdr.h"
#include "ReservedAreaId.h"
#include "ReservedFieldId.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new DeleteField("P113-18:P113-18"));
    add(new NoCapability("P113-19:P113-20"));
    add(new IdrNotPresent("P113-22:P113-22"));
    add(new AreaNotPresent("P113-23:P113-23"));
    add(new FieldNotPresent("P113-23:P113-24"));
    add(new ReadOnlyField("P113-25:P113-25"));
    add(new ReadOnlyArea("P113-25:P113-25"));
    add(new ReadOnlyIdr("P113-25:P113-25"));
    add(new ReservedAreaId("P113-26:P113-27"));
    add(new ReservedFieldId("P113-26:P113-27"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrFieldDelete";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrFieldDelete().";
}

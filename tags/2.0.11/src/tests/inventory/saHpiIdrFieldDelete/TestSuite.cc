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
#include "ReservedAreaId.h"
#include "ReservedFieldId.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrFieldDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new DeleteField("B.02|P137-18:P137-18"));
    add(new NoCapability("B.02|P137-19:P137-20"));
    add(new IdrNotPresent("B.02|P137-22:P137-22"));
    add(new AreaNotPresent("B.02|P137-23:P137-23"));
    add(new FieldNotPresent("B.02|P137-23:P137-24"));
    add(new ReadOnlyField("B.02|P137-25:P137-25"));
    add(new ReadOnlyArea("B.02|P137-25:P137-25"));
    add(new ReservedAreaId("B.02|P137-26:P137-27"));
    add(new ReservedFieldId("B.02|P137-26:P137-27"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));


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

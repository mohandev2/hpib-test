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
#include "DeleteArea.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaIdNotPresent.h"
#include "ReservedAreaId.h"
#include "ReadOnlyArea.h"
#include "ReadOnlyField.h"
#include "ReadOnlyIdr.h"
#include "VerifyDeletedFields.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrAreaDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new DeleteArea("P106-17:P106-17"));
    add(new NoCapability("P106-18:P106-19"));
    add(new IdrNotPresent("P106-21:P106-21"));
    add(new AreaIdNotPresent("P106-22:P106-22"));
    add(new ReservedAreaId("P106-23:P106-24"));
    add(new ReadOnlyArea("P106-26:P106-26"));
    add(new ReadOnlyField("P106-27:P106-27"));
    add(new ReadOnlyIdr("P106-28:P106-28"));
    add(new VerifyDeletedFields("P106-30:P106-30"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrAreaDelete";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrAreaDelete().";
}

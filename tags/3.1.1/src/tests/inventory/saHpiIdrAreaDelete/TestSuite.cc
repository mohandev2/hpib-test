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

    add(new DeleteArea("B.02|P128-17:P128-17"));
    add(new NoCapability("B.02|P128-18:P128-19"));
    add(new IdrNotPresent("B.02|P128-21:P128-21"));
    add(new AreaIdNotPresent("B.02|P128-22:P128-22"));
    add(new ReservedAreaId("B.02|P128-23:P128-24"));
    add(new ReadOnlyArea("B.02|P128-26:P128-26"));
    add(new ReadOnlyField("B.02|P128-27:P128-27"));
    add(new ReadOnlyIdr("B.02|P128-28:P128-28"));
    add(new VerifyDeletedFields("B.02|P128-30:P128-30"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

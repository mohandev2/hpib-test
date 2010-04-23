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
#include "AddArea.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "UnspecifiedAreaType.h"
#include "NullAreaId.h"
#include "InvalidAreaType.h"
#include "ReadOnly.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrAreaAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddArea("P104-18:P104-18; P104-32-P104-33"));
    add(new NoCapability("P104-19:P104-20"));
    add(new IdrNotPresent("P104-21:P104-21"));
    add(new UnspecifiedAreaType("P104-22:P104-23"));
    add(new NullAreaId("P104-28:P104-28"));
    add(new InvalidAreaType("P104-29:P104-29"));
    add(new ReadOnly("P104-30:P104-30"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrAreaAdd";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrAreaAdd().";
}

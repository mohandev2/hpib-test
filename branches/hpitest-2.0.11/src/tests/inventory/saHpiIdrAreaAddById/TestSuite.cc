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
 *     Akmal A. Muqeeth <aax6@unh.edu>
 */

#include "TestSuite.h"
#include "AddArea.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "UnspecifiedAreaType.h"
#include "InvalidAreaId.h"
#include "DuplicateArea.h"
#include "InvalidAreaType.h"
#include "ReadOnly.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrAreaAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new AddArea("B.02|P126-18:P126-18; P126-32-P126-33"));
    add(new NoCapability("B.02|P126-19:P126-20"));
    add(new IdrNotPresent("B.02|P126-21:P126-21"));
    add(new UnspecifiedAreaType("B.02|P126-22:P126-23"));
    add(new InvalidAreaId("B.02|P126-28:P126-28"));
    add(new DuplicateArea("B.02|P126-28:P126-28"));
    add(new InvalidAreaType("B.02|P126-29:P126-29"));
    add(new ReadOnly("B.02|P126-30:P126-30"));
    add(new InvalidResource("B.02|P29-40:P29-42"));
    add(new InvalidSession("B.02|P29-43:P29-45"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrAreaAddById";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrAreaAddById().";
}

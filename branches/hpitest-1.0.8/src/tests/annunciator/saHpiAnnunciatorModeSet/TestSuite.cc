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
#include "SetMode.h"
#include "NoCapability.h"
#include "AnnunciatorNotPresent.h"
#include "InvalidMode.h"
#include "ReadOnly.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiAnnunciatorModeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SetMode("P131-24:P131-24"));
    add(new NoCapability("P131-25:P131-26"));
    add(new AnnunciatorNotPresent("P131-27:P131-28"));
    add(new InvalidMode("P131-29:P131-29"));
    add(new ReadOnly("P131-30:P131-30"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAnnunciatorModeSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAnnunciatorModeSet().";
}

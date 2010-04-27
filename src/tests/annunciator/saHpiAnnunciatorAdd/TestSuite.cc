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
#include "AddAnnouncement.h"
#include "NoCapability.h"
#include "AnnunciatorNotPresent.h"
#include "NullAnnouncement.h"
#include "InvalidSeverity.h"
#include "InvalidStatusCond.h"
#include "ReadOnly.h"
#include "VerifyFields.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddAnnouncement("P127-17:P127-17"));
    add(new NoCapability("P127-18:P127-19"));
    add(new AnnunciatorNotPresent("P127-20:P127-21"));
    add(new NullAnnouncement("P127-23:P127-23"));
    add(new InvalidSeverity("P127-24:P127-24"));
    add(new InvalidStatusCond("P127-25:P127-25"));
    add(new ReadOnly("P127-28:P127-28"));
    add(new VerifyFields("P127-30:P127-32"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAnnunciatorAdd";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAnnunciatorAdd().";
}

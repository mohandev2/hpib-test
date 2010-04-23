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
#include "GetAll.h"
#include "SeverityTraversal.h"
#include "UnacknowledgedTraversal.h"
#include "NoCapability.h"
#include "InvalidSeverity.h"
#include "AnnunciatorNotPresent.h"
#include "NoSeverityAnnouncements.h"
#include "NoSeverityUnackAnnouncements.h"
#include "NullAnnouncement.h"
#include "Mismatch.h"
#include "DeletedAnnouncement.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiAnnunciatorGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new GetAll("P122-24:P122-24"));
    add(new SeverityTraversal("P122-17:P122-18"));
    add(new UnacknowledgedTraversal("P122-19:P122-20"));
    add(new NoCapability("P122-25:P122-26"));
    add(new InvalidSeverity("P122-27:P122-27"));
    add(new AnnunciatorNotPresent("P122-29:P122-29"));
    add(new NoSeverityAnnouncements("P122-33:P122-35"));
    add(new NoSeverityUnackAnnouncements("P122-33:P122-35"));
    add(new NullAnnouncement("P122-36:P122-36"));
    add(new Mismatch("P122-37:P122-38"));
    add(new DeletedAnnouncement("P123-6:P123-7"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAnnunciatorGetNext";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAnnunciatorGetNext().";
}

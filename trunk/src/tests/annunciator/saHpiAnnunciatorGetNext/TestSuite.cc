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

    add(new GetAll("B.02|P146-24:P146-24"));
    add(new SeverityTraversal("B.02|P146-17:P146-18"));
    add(new UnacknowledgedTraversal("B.02|P146-19:P146-20"));
    add(new NoCapability("B.02|P146-25:P146-26"));
    add(new InvalidSeverity("B.02|P146-27:P146-27"));
    add(new AnnunciatorNotPresent("B.02|P146-29:P146-29"));
    add(new NoSeverityAnnouncements("B.02|P146-33:P146-35"));
    add(new NoSeverityUnackAnnouncements("B.02|P146-33:P146-35"));
    add(new NullAnnouncement("B.02|P146-36:P146-36"));
    add(new Mismatch("B.02|P146-37:P146-38"));
    add(new DeletedAnnouncement("B.02|P123-6:P123-7"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));
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

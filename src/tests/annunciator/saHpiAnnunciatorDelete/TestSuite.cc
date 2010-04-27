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
#include "DeleteAnnouncement.h"
#include "SeverityDelete.h"
#include "IgnoreSeverity.h"
#include "NoCapability.h"
#include "AnnunciatorNotPresent.h"
#include "AnnouncementNotPresent.h"
#include "InvalidSeverity.h"
#include "ReadOnly.h"
#include "DeleteAll.h"
#include "NoSeverity.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiAnnunciatorDelete;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new DeleteAnnouncement("P128-20:P128-20"));
    add(new SeverityDelete("P128-2:P128-4"));
    add(new NoCapability("P128-21:P128-22"));
    add(new InvalidSeverity("P128-25:P128-26"));
    add(new AnnunciatorNotPresent("P128-28:P128-28"));
    add(new AnnouncementNotPresent("P128-29:P128-30"));
    add(new ReadOnly("P128-31:P128-31"));
    add(new IgnoreSeverity("P128-34:P128-35"));
    add(new DeleteAll("P128-36:P128-38"));
    add(new NoSeverity("P129-1:P129-2"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAnnunciatorDelete";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAnnunciatorDelete().";
}

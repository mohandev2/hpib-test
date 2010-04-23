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
#include "AcknowledgeAnnouncement.h"
#include "NoCapability.h"
#include "AnnunciatorNotPresent.h"
#include "AnnouncementNotPresent.h"
#include "InvalidSeverity.h"
#include "AcknowledgeAll.h"
#include "AcknowledgeSeverity.h"
#include "AcknowledgeTwice.h"
#include "IgnoreSeverity.h"
#include "NoSeverity.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiAnnunciatorAcknowledge;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AcknowledgeAnnouncement("P125-21:P125-21;P125-34:P125-34"));
    add(new NoCapability("P125-22:P125-23"));
    add(new AnnunciatorNotPresent("P125-25:P125-25"));
    add(new AnnouncementNotPresent("P125-26:P125-26"));
    add(new InvalidSeverity("P125-27:P125-28"));
    add(new AcknowledgeAll("P125-35:P125-36"));
    add(new AcknowledgeSeverity("P126-1:P126-2"));
    add(new AcknowledgeTwice("P126-6:P126-7"));
    add(new IgnoreSeverity("P126-8:P126-8"));
    add(new NoSeverity("P126-9:P126-10"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiAnnunciatorAcknowledge";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiAnnunciatorAcknowledge().";
}

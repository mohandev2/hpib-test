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
    add(new AcknowledgeAnnouncement("B.02|P149-21:P149-21;P149-34:P149-34"));
    add(new NoCapability("B.02|P149-22:P149-23"));
    add(new AnnunciatorNotPresent("B.02|P149-25:P149-25"));
    add(new AnnouncementNotPresent("B.02|P149-26:P149-26"));
    add(new InvalidSeverity("B.02|P149-27:P149-28"));
    add(new AcknowledgeAll("B.02|P149-35:P149-36"));
    add(new AcknowledgeSeverity("B.02|P150-1:P150-2"));
    add(new AcknowledgeTwice("B.02|P150-6:P150-7"));
    add(new IgnoreSeverity("B.02|P150-8:P150-8"));
    add(new NoSeverity("B.02|P150-9:P150-10"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

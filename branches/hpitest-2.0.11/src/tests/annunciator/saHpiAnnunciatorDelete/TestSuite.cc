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

    add(new DeleteAnnouncement("B.02|P153-20:P153-20"));
    add(new SeverityDelete("B.02|P153-2:P153-4"));
    add(new NoCapability("B.02|P153-21:P153-22"));
    add(new InvalidSeverity("B.02|P153-25:P153-26"));
    add(new AnnunciatorNotPresent("B.02|P153-28:P153-28"));
    add(new AnnouncementNotPresent("B.02|P153-29:P153-30"));
    add(new ReadOnly("B.02|P153-31:P153-31"));
    add(new IgnoreSeverity("B.02|P153-34:P153-35"));
    add(new DeleteAll("B.02|P153-36:P153-38"));
    add(new NoSeverity("B.02|P154-1:P154-2"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

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
#include "InvalidNameLength.h"
#include "InvalidStatusCond.h"
#include "ReadOnly.h"
#include "VerifyFields.h"
#include "InvalidResource.h"
#include "InvalidSession.h"
#include "InvalidTextCharacters.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddAnnouncement("B.02|P151-17:P151-17"));
    add(new NoCapability("B.02|P151-18:P151-19"));
    add(new AnnunciatorNotPresent("B.02|P151-20:P151-21"));
    add(new NullAnnouncement("B.02|P151-23:P151-23"));
    add(new InvalidSeverity("B.02|P151-24:P151-24"));
    add(new InvalidNameLength("B.02|P151-26:P151-26"));
    add(new InvalidTextCharacters("B.02|P151-26:P151-26"));
    add(new InvalidStatusCond("B.02|P151-25:P151-25"));	
    add(new ReadOnly("B.02|P151-36:P151-36"));
    add(new VerifyFields("B.02|P152-2:P152-2"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));
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

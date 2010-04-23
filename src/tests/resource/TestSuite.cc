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
 *
 * Changes:
 * 2009/05/20 Lars.Wetzel@emerson.com
 *            Add saHpiGetChildEntityPath test cases
 * 2009/10/08 larswetzel@users.sourceforge.net
 *            Add saHpiMyEntityPathGet test cases
 */

#include "TestSuite.h"
#include "saHpiRptEntryGet/TestSuite.h"
#include "saHpiRptEntryGetByResourceId/TestSuite.h"
#include "saHpiResourceSeveritySet/TestSuite.h"
#include "saHpiResourceTagSet/TestSuite.h"
#include "saHpiResourceIdGet/TestSuite.h"
#include "saHpiRptEntryGetByResourceId/TestSuite.h"
#include "saHpiResourceFailedRemove/TestSuite.h"
#include "saHpiGetIdByEntityPath/TestSuite.h"
#include "saHpiMyEntityPathGet/TestSuite.h"
#include "saHpiGetChildEntityPath/TestSuite.h"

using namespace ns_resource;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() { 
    add(new ns_saHpiRptEntryGet::TestSuite());
    add(new ns_saHpiRptEntryGetByResourceId::TestSuite());
    add(new ns_saHpiResourceSeveritySet::TestSuite());
    add(new ns_saHpiResourceTagSet::TestSuite());
    add(new ns_saHpiResourceIdGet::TestSuite());
    add(new ns_saHpiRptEntryGetByResourceId::TestSuite());
    add(new ns_saHpiResourceFailedRemove::TestSuite());
    add(new ns_saHpiGetIdByEntityPath::TestSuite());
    add(new ns_saHpiMyEntityPathGet::TestSuite());
    add(new ns_saHpiGetChildEntityPath::TestSuite());
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "Resource";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Resource Management.";
}

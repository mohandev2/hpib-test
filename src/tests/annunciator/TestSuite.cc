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
#include "saHpiAnnunciatorGetNext/TestSuite.h"
#include "saHpiAnnunciatorGet/TestSuite.h"
#include "saHpiAnnunciatorAcknowledge/TestSuite.h"
#include "saHpiAnnunciatorAdd/TestSuite.h"
#include "saHpiAnnunciatorDelete/TestSuite.h"
#include "saHpiAnnunciatorModeGet/TestSuite.h"
#include "saHpiAnnunciatorModeSet/TestSuite.h"

using namespace ns_annunciator;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() { 
    add(new ns_saHpiAnnunciatorGetNext::TestSuite());
    add(new ns_saHpiAnnunciatorGet::TestSuite());
    add(new ns_saHpiAnnunciatorAcknowledge::TestSuite());
    add(new ns_saHpiAnnunciatorAdd::TestSuite());
    add(new ns_saHpiAnnunciatorDelete::TestSuite());
    add(new ns_saHpiAnnunciatorModeGet::TestSuite());
    add(new ns_saHpiAnnunciatorModeSet::TestSuite());
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "Annunciator";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Annunicator.";
}

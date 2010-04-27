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
 *     Akmal Muqeeth <aax6@unh.edu>
 */

#include "TestSuite.h"
#include "saHpiIdrInfoGet/TestSuite.h"
#include "saHpiIdrAreaHeaderGet/TestSuite.h"
#include "saHpiIdrAreaAdd/TestSuite.h"
#include "saHpiIdrAreaAddById/TestSuite.h"
#include "saHpiIdrAreaDelete/TestSuite.h"
#include "saHpiIdrFieldGet/TestSuite.h"
#include "saHpiIdrFieldAdd/TestSuite.h"
#include "saHpiIdrFieldAddById/TestSuite.h"
#include "saHpiIdrFieldSet/TestSuite.h"
#include "saHpiIdrFieldDelete/TestSuite.h"



using namespace ns_inventory;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() { 
    add(new ns_saHpiIdrInfoGet::TestSuite());
    add(new ns_saHpiIdrAreaHeaderGet::TestSuite());
    add(new ns_saHpiIdrAreaAdd::TestSuite());
    add(new ns_saHpiIdrAreaAddById::TestSuite());
    add(new ns_saHpiIdrAreaDelete::TestSuite());
    add(new ns_saHpiIdrFieldGet::TestSuite());
    add(new ns_saHpiIdrFieldAdd::TestSuite());
    add(new ns_saHpiIdrFieldAddById::TestSuite());
    add(new ns_saHpiIdrFieldSet::TestSuite());
    add(new ns_saHpiIdrFieldDelete::TestSuite());
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "Inventory";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Inventory Data Repositories.";
}

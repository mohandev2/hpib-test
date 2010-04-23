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
#include "Insert.h"
#include "Extract.h"
#include "NoCapability.h"
#include "NoCapability.h"
#include "NotPending.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiResourceActiveSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
/*    add(new Insert("P139-2:P139-3;P139-13:P139-13;P139-20:P139-28"));
    add(new Extract("P139-2:P139-3;P139-13:P139-13;P139-29:P139-30"));
    add(new NoCapability("P139-14:P139-15"));
    add(new NotPending("P139-17:P139-17"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
*/

    add(new Insert("B.02|P194-2:P194-3;P194-13:P194-13;P194-20:P194-28"));
    add(new Extract("B.02|P194-2:P194-3;P194-13:P194-13;P194-29:P194-30"));
    add(new NoCapability("B.02|P194-14:P194-15"));
    add(new NotPending("B.02|P194-17:P194-17"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P29-18"));

}


/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiResourceActiveSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiResourceActiveSet().";
}

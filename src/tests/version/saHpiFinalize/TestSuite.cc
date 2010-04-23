/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 */

#include "TestSuite.h"
#include "Finalize.h"
#include "InvalidRequest.h"

using namespace ns_saHpiFinalize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
  
   add(new Finalize("B.03|P50-6:P50-6"));
   add(new InvalidRequest("B.03|P50-7:P50-7"));
}

/*****************************************************************************
 * Return the name of the HPI function being tested.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiFinalize";
}

/*****************************************************************************
 * Return the description.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Verfiy saHpiFinalize().";
}

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
#include "Initialize.h"
#include "BackwardCompatibility.h"
#include "InvalidRequest.h"
#include "InvalidVersion.h"
#include "InvalidParams.h"
#include "InvalidData.h"

using namespace ns_saHpiInitialize;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
  
   add(new Initialize("B.03|P48-6:P48-6"));
   add(new BackwardCompatibility("B.03|P49-4:P49-8"));
   add(new InvalidRequest("B.03|P48-34:P48-34"));
   add(new InvalidVersion("B.03|P48-29:P48-29"));
   add(new InvalidParams("B.03|P48-30:P48-30"));
   add(new InvalidData("B.03|P48-32:P48-32"));
}

/*****************************************************************************
 * Return the name of the HPI function being tested.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiInitialize";
}

/*****************************************************************************
 * Return the description.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Verfiy saHpiInitialize().";
}

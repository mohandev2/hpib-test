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
 *     Meghana Srivatsavai <mjn77@unh.edu>
 * 
 * Changes:
 * 09/12/28 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Integrate the test case into hpitest 
 * 
 */

#include "TestSuite.h"
#include "GetDisable.h"
#include "NoCapability.h"
#include "FumiNotPresent.h"
#include "NullDisable.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiFumiAutoRollbackDisableGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new GetDisable("B.03|P212-18:P62-18"));
    add(new NoCapability("B.03|P212-19:P62-20"));
    add(new NullDisable("B.03|P212-22:P62-22"));
    add(new FumiNotPresent("B.03|P212-22:P62-22"));
    add(new InvalidResource("B.03|P212-15:P36-16"));
    add(new InvalidSession("B.03|P212-17:P36-18"));

}


/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiFumiAutoRollbackDisableGet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiFumiAutoRollbackDisableGet().";
}

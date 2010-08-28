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
 * 
 * Changes
 * 09/12/28 Lars Wetzel <larswetzel@users.sourceforge.net>
 * 	        Add saHpiFumiAutoRollbackDisableGet and saHpiFumiAutoRollbackDisableSet
 */

#include "TestSuite.h"
#include "saHpiFumiLogicalTargetInfoGet/TestSuite.h"
#include "saHpiFumiServiceImpactGet/TestSuite.h"
#include "saHpiFumiSpecInfoGet/TestSuite.h"
#include "saHpiFumiAutoRollbackDisableGet/TestSuite.h"
#include "saHpiFumiAutoRollbackDisableSet/TestSuite.h"

using namespace ns_fumi;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() { 
	add(new ns_saHpiFumiLogicalTargetInfoGet::TestSuite());
	add(new ns_saHpiFumiServiceImpactGet::TestSuite());
	add(new ns_saHpiFumiSpecInfoGet::TestSuite());
	add(new ns_saHpiFumiAutoRollbackDisableGet::TestSuite());
	add(new ns_saHpiFumiAutoRollbackDisableSet::TestSuite());
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "Fumi";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Fumi.";
}

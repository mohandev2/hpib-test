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
#include "saHpiHotSwapPolicyCancel/TestSuite.h"
#include "saHpiResourceActiveSet/TestSuite.h"
#include "saHpiResourceInactiveSet/TestSuite.h"
#include "saHpiAutoInsertTimeoutGet/TestSuite.h"
#include "saHpiAutoInsertTimeoutSet/TestSuite.h"
#include "saHpiAutoExtractTimeoutGet/TestSuite.h"
#include "saHpiAutoExtractTimeoutSet/TestSuite.h"
#include "saHpiHotSwapStateGet/TestSuite.h"
#include "saHpiHotSwapActionRequest/TestSuite.h"
#include "saHpiHotSwapIndicatorStateGet/TestSuite.h"
#include "saHpiHotSwapIndicatorStateSet/TestSuite.h"

using namespace ns_hotswap;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() { 
    add(new ns_saHpiHotSwapPolicyCancel::TestSuite());
    add(new ns_saHpiResourceActiveSet::TestSuite());
    add(new ns_saHpiResourceInactiveSet::TestSuite());
    add(new ns_saHpiAutoInsertTimeoutGet::TestSuite());
    add(new ns_saHpiAutoInsertTimeoutSet::TestSuite());
    add(new ns_saHpiAutoExtractTimeoutGet::TestSuite());
    add(new ns_saHpiAutoExtractTimeoutSet::TestSuite());
    add(new ns_saHpiHotSwapStateGet::TestSuite());
    add(new ns_saHpiHotSwapActionRequest::TestSuite());
    add(new ns_saHpiHotSwapIndicatorStateGet::TestSuite());
    add(new ns_saHpiHotSwapIndicatorStateSet::TestSuite());
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "HotSwap";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for Managed Hot Swap.";
}

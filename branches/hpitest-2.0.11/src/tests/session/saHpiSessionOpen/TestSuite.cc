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
#include "UnspecifiedDomainId.h"
#include "InvalidDomainId.h"
#include "NonNullSecurityParams.h"
#include "NullSessionId.h"
#include "ValidDomainId.h"

using namespace ns_saHpiSessionOpen;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new UnspecifiedDomainId("B.02|P40-17:P40-17"));
    add(new ValidDomainId("B.02|P40-2:P22-2;P40-17:P40-17"));
    add(new InvalidDomainId("B.02|P40-18:P40-18"));
    add(new NonNullSecurityParams("B.02|P40-19:P40-20"));
    add(new NullSessionId("B.02|P40-21:P40-21"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiSessionOpen";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiSessionOpen().";
}

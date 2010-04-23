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
#include "SetTag.h"
#include "InvalidSession.h"
#include "InvalidResource.h"
#include "InvalidDataType.h"
#include "InvalidDataField.h"
#include "InvalidLanguage.h"
#include "NullResourceTag.h"
#include "Scope.h"

using namespace ns_saHpiResourceTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SetTag("P44-14:P44-14"));
    add(new InvalidDataType("P44-17:P44-17"));
    add(new InvalidDataField("P44-18:P44-18"));
    add(new InvalidLanguage("P44-19:P44-20"));
    add(new NullResourceTag("P44-21:P44-21"));
    add(new Scope("P44-27:P44-28"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiResourceTagSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiResourceTagSet().";
}

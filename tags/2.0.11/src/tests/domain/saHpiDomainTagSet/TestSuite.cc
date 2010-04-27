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
#include "SetDomainTag.h"
#include "InvalidSession.h"
#include "InvalidDataType.h"
#include "InvalidDataField.h"
#include "InvalidLanguage.h"
#include "NullDomainTag.h"
#include "TwoSessions.h"

using namespace ns_saHpiDomainTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

 add(new SetDomainTag("B.02|P45-13:P36-13"));
    add(new InvalidDataType("B.02|P45-16:P45-16"));
    add(new InvalidDataField("B.02|P45-17:P45-17"));
    add(new InvalidLanguage("B.02|P45-18:P45-19"));
    add(new NullDomainTag("B.02|P45-20:P45-20"));
    add(new TwoSessions("B.02|P16-25:P16-25"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiDomainTagSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiDomainTagSet().";
}

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
 * Modified:
 *     AKMAL MUQEETH
 *     PUSHPA DATLA    
 */

#include "TestSuite.h"
#include "SetField.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaNotPresent.h"
#include "FieldNotPresent.h"
#include "ReadOnlyField.h"
#include "InvalidFieldType.h"
#include "UnspecifiedFieldType.h"
#include "NullField.h"
#include "InvalidDataType.h"
#include "InvalidTextCharacters.h"
#include "InvalidLanguage.h"
#include "IgnoreReadOnly.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrFieldSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new SetField("B.02|P135-16:P135-16;P136-6:P136-6"));
    add(new NoCapability("B.02|P135-17:P135-18"));
    add(new IdrNotPresent("B.02|P135-20:P135-20"));
    add(new AreaNotPresent("B.02|P135-21:P135-22"));
    add(new FieldNotPresent("B.02|P135-21:P135-22"));
    add(new NullField("B.02|P135-24:P135-24"));
    add(new InvalidFieldType("B.02|P135-25:P135-25"));
    add(new UnspecifiedFieldType("B.02|P135-26:P135-26"));
    add(new ReadOnlyField("B.02|P135-32:P135-32"));
    add(new InvalidDataType("B.02|P136-1:P136-1"));
    add(new InvalidTextCharacters("B.02|P136-2:P136-2"));
    add(new InvalidLanguage("B.02|P136-3:P136-4"));
    add(new IgnoreReadOnly("B.02|P136-7:P136-8"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrFieldSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrFieldSet().";
}

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
#include "SetField.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaNotPresent.h"
#include "FieldNotPresent.h"
#include "ReadOnlyField.h"
#include "ReadOnlyArea.h"
#include "ReadOnlyIdr.h"
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
    add(new SetField("P111-16:P111-16;P112-6:P112-6"));
    add(new NoCapability("P111-17:P111-18"));
    add(new IdrNotPresent("P111-20:P111-20"));
    add(new AreaNotPresent("P111-21:P111-22"));
    add(new FieldNotPresent("P111-21:P111-22"));
    add(new NullField("P111-24:P111-24"));
    add(new InvalidFieldType("P111-25:P111-25"));
    add(new UnspecifiedFieldType("P111-26:P111-26"));
    add(new ReadOnlyField("P111-32:P111-32"));
    add(new ReadOnlyArea("P111-33:P111-33"));
    add(new ReadOnlyIdr("P111-34:P111-34"));
    add(new InvalidDataType("P112-1:P112-1"));
    add(new InvalidTextCharacters("P112-2:P112-2"));
    add(new InvalidLanguage("P112-3:P112-4"));
    add(new IgnoreReadOnly("P112-7:P112-8"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
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

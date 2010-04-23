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
#include "AddField.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaNotPresent.h"
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

using namespace ns_saHpiIdrFieldAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new AddField("P109-17:P109-17;P110-5:P110-6;P110-7:P110-8"));
    add(new NoCapability("P109-18:P109-19"));
    add(new IdrNotPresent("P109-21:P109-21"));
    add(new AreaNotPresent("P109-22:P109-22"));
    add(new ReadOnlyArea("P109-28:P109-28"));
    add(new ReadOnlyIdr("P109-29:P109-29"));
    add(new InvalidFieldType("P109-31:P109-31"));
    add(new UnspecifiedFieldType("P109-32:P109-32"));
    add(new NullField("P110-3:P110-3"));
    add(new InvalidDataType("P109-34:P109-34"));
    add(new InvalidTextCharacters("P109-35:P109-35"));
    add(new InvalidLanguage("P110-1:P110-2"));
    add(new IgnoreReadOnly("P110-9:P110-10;"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrFieldAdd";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrFieldAdd().";
}

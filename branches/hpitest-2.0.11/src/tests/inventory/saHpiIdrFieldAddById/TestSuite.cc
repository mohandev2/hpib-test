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
 * Modified By:
 *   AKMAL MUQEETH
 */

#include "TestSuite.h"
#include "AddField.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "ReadOnlyArea.h"
#include "InvalidFieldType.h"
#include "UnspecifiedFieldType.h"
#include "NullField.h"
#include "DuplicateField.h"
#include "InvalidDataType.h"
#include "InvalidLanguage.h"
#include "IgnoreReadOnly.h"
#include "InvalidSession.h"
#include "InvalidResource.h"

using namespace ns_saHpiIdrFieldAddById;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {

    add(new AddField("B.02|P133-17:P133-17;P133-5:P133-6;P133-7:P133-8"));
    add(new NoCapability("B.02|P133-18:P13319"));
    add(new IdrNotPresent("B.02|P133-21:P133-21"));
    add(new ReadOnlyArea("B.02|P133-28:P133-28"));
    add(new InvalidFieldType("B.02|P133-31:P133-31"));
    add(new UnspecifiedFieldType("B.02|P133-32:P133-32"));
    add(new NullField("B.02|P133-3:P133-3"));
    add(new DuplicateField("B.02|P133-3:P133-3"));
    add(new InvalidDataType("B.02|P133-34:P133-34"));
    add(new InvalidLanguage("B.02|P133-1:P133-2"));
    add(new IgnoreReadOnly("B.02|P133-9:P133-10;"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiIdrFieldAddById";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiIdrFieldAddById().";
}

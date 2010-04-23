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
 *   PUSHPA DATLA
 */

#include "TestSuite.h"
#include "AddField.h"
#include "NoCapability.h"
#include "IdrNotPresent.h"
#include "AreaNotPresent.h"
#include "ReadOnlyArea.h"
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

    add(new AddField("B.02|P131-17:P131-17;P132-5:P132-6;P132-7:P132-8"));
    add(new NoCapability("B.02|P131-18:P131-19"));
    add(new IdrNotPresent("B.02|P131-21:P131-21"));
    add(new AreaNotPresent("B.02|P131-22:P131-22"));
    add(new ReadOnlyArea("B.02|P131-28:P131-28"));
    add(new InvalidFieldType("B.02|P131-31:P131-31"));
    add(new UnspecifiedFieldType("B.02|P131-32:P131-32"));
    add(new NullField("B.02|P132-3:P132-3"));
    add(new InvalidDataType("B.02|P131-34:P131-34"));
    add(new InvalidTextCharacters("B.02|P131-35:P131-35"));
    add(new InvalidLanguage("B.02|P132-1:P132-2"));
    add(new IgnoreReadOnly("B.02|P132-9:P132-10;"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

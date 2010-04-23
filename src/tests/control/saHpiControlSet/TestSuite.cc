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
#include "SetControl.h"
#include "SingleLine.h"
#include "ControlNotPresent.h"
#include "NoCapability.h"
#include "Mid.h"
#include "InvalidType.h"
#include "InvalidAnalog.h"
#include "InvalidDataType.h"
#include "InvalidDataLength.h"
#include "InvalidLanguage.h"
#include "InvalidCtrlMode.h"
#include "NullCtrlState.h"
#include "InvalidDigital.h"
#include "InvalidStreamLength.h"
#include "InvalidTextCharacters.h"
#include "PulseOn.h"
#include "PulseOff.h"
#include "ReadOnly.h"
#include "IgnoreState.h"
#include "ClearSpaces.h"
#include "ClearLine.h"
#include "ClearAllLinesAndWrite.h"
#include "ClearAllLines.h"
#include "WrapText.h"
#include "SetAnalog.h"
#include "SetDigital.h"
#include "SetDiscrete.h"
#include "SetStream.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiControlSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new SetControl("P96-18:P96-18"));
    add(new SetAnalog("P96-18:P96-18"));
    add(new SetDigital("P96-18:P96-18"));
    add(new SetDiscrete("P96-18:P96-18"));
    add(new SetStream("P96-18:P96-18"));
    add(new NoCapability("P96-19:P96-20"));
    add(new ControlNotPresent("P96-21:P96-21"));
    add(new InvalidType("P96-23:P96-23"));
    add(new InvalidAnalog("P96-24:P96-24"));
    add(new InvalidDataLength("P96-25:P96-26"));
    add(new InvalidDataType("P96-27:P96-27"));
    add(new InvalidLanguage("P96-28:P96-29"));
    add(new InvalidCtrlMode("P96-32:P96-32"));
    add(new NullCtrlState("P96-33:P96-33"));
    add(new InvalidDigital("P96-34:P96-34"));
    add(new InvalidStreamLength("P96-35:P96-35"));
    add(new InvalidTextCharacters("P96-36:P96-37"));
    add(new PulseOn("P97-1:P97-2"));
    add(new PulseOff("P97-2:P97-3"));
    add(new ReadOnly("P97-4:P97-4"));
    add(new IgnoreState("P97-6:P97-7; P97-9:P97-10"));
    add(new ClearSpaces("P97-12:P97-13"));
    add(new ClearLine("P97-13:P97-13"));
    add(new WrapText("P97-14:P97-15"));
    add(new ClearAllLinesAndWrite("P97-19:P97-21"));
    add(new ClearAllLines("P97-22:P97-24"));
    add(new SingleLine("P97-10:P97-11"));
    add(new Mid("P97-26:P97-27"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiControlSet";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiControlSet().";
}

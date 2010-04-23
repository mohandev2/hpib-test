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


add(new SetControl("B.02|P114-18:P114-18"));
    add(new SetAnalog("B.02|P114-18:P114-18"));
    add(new SetDigital("B.02|P114-18:P114-18"));
    add(new SetDiscrete("B.02|P114-18:P114-18"));
    add(new SetStream("B.02|P114-18:P114-18"));
    add(new NoCapability("B.02|P114-19:P114-20"));
    add(new ControlNotPresent("B.02|P114-21:P114-21"));
    add(new InvalidType("B.02|P114-23:P114-23"));
    add(new InvalidAnalog("B.02|P114-24:P114-24"));
    add(new InvalidDataLength("B.02|P114-25:P114-26"));
    add(new InvalidDataType("B.02|P114-27:P114-27"));
    add(new InvalidLanguage("B.02|P114-28:P114-29"));
    add(new InvalidCtrlMode("B.02|P114-32:P114-32"));
    add(new NullCtrlState("B.02|P114-33:P114-33"));
    add(new InvalidDigital("B.02|P114-34:P114-34"));
    add(new InvalidStreamLength("B.02|P114-35:P114-35"));
    add(new InvalidTextCharacters("B.02|P115-1:P115-2"));
    add(new PulseOn("B.02|P115-3:P115-4"));
    add(new PulseOff("B.02|P115-4:P115-5"));
    add(new ReadOnly("B.02|P115-6:P115-7"));
    add(new IgnoreState("B.02|P115-8:P115-9; P115-11:P115-12"));
    add(new ClearSpaces("B.02|P115-14:P115-15"));
    add(new ClearLine("B.02|P115-15:P115-16"));
    add(new WrapText("B.02|P115-17:P115-18"));
    add(new ClearAllLinesAndWrite("B.02|P115-21:P115-23"));
    add(new ClearAllLines("B.02|P115-24:P115-26"));
    add(new SingleLine("B.02|P115-12:P115-13"));
    add(new Mid("B.02|P115-28:P115-29"));
    add(new InvalidResource("B.02|P36-15:P36-16"));
    add(new InvalidSession("B.02|P36-17:P36-18"));

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

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
#include "Toggle.h"
#include "NoCapability.h"
#include "InvalidAction.h"
#include "InvalidInsertion.h"
#include "InvalidExtraction.h"
#include "InvalidResource.h"
#include "InvalidSession.h"

using namespace ns_saHpiHotSwapActionRequest;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

TestSuite::TestSuite() {
    add(new Toggle("P148-2:P148-2;P148-16:P148-16;P148-16:P148-16;"
                   "P136-5:P136-6;P136-15:P136-16;P136-20:P136-22;"
                   "P136-31:P136-32"));
    add(new NoCapability("P148-17:P148-18"));
    add(new InvalidAction("P148-21:P148-21"));
    add(new InvalidInsertion("P148-19:P148-20;P148-28:P148-29"));
    add(new InvalidExtraction("P148-19:P148-20;P148-28:P148-29"));
    add(new InvalidResource("P29-40:P29-42"));
    add(new InvalidSession("P29-43:P29-45"));
}

/*****************************************************************************
 * Return the name of the test suite.
 *****************************************************************************/

const char *TestSuite::getName() {
    return "saHpiHotSwapActionRequest";
}

/*****************************************************************************
 * Return the description of the test suite.
 *****************************************************************************/

const char *TestSuite::getDescription() {
    return "Conformance test suite for saHpiHotSwapActionRequest().";
}

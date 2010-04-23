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

#include "AddEvent.h"
#include "EventHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AddEvent::AddEvent(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AddEvent::getName() {
    return "AddEvent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AddEvent::getDescription() {
    return "Add user events for each of the supported DataTypes.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AddEvent::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AddEvent::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiTextBufferT buf;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE,
                                  SAHPI_TL_TYPE_BCDPLUS,
                                  SAHPI_TL_TYPE_ASCII6,
                                  SAHPI_TL_TYPE_TEXT,
                                  SAHPI_TL_TYPE_BINARY };

    for (int i = 0; i < 5; i++) {
        TextBufferHelper::fillByDataType(&buf, dataType[i]);
        EventHelper::fill(&event, &buf);
        SaErrorT error = saHpiEventAdd(sessionId, &event);
        if (error == SA_OK) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_ADD, SA_OK, error,
                                 "DataType: %s", HpiString::dataType(dataType[i]));
        }
    }

    return status;
}

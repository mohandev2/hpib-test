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

#include "InvalidDataField.h"
#include "TextBufferHelper.h"
#include "EventHelper.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidDataField::InvalidDataField(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidDataField::getName() {
    return "InvalidDataField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidDataField::getDescription() {
    return "The <i>Data</i> field contains characters that are not legal\n"
           "according the value of <i>DataType</i>.  The tested DataTypes\n"
           "are SAHPI_TL_TYPE_UNICODE, SAHPI_TL_TYPE_ASCII6, and\n"
           "SAHPI_TL_TYPE_BCDPLUS.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidDataField::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidDataField::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiTextBufferT buf;
    SaHpiEventT event;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE,
                                  SAHPI_TL_TYPE_ASCII6,
                                  SAHPI_TL_TYPE_BCDPLUS };

    for (int i = 0; i < 3; i++) {
        TextBufferHelper::fillInvalidData(&buf, dataType[i]);
        EventHelper::fill(&event, &buf);
        SaErrorT error = saHpiEventAdd(sessionId, &event);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_ADD, SA_ERR_HPI_INVALID_PARAMS, error,
                                 "DataType: %s", HpiString::dataType(dataType[i]));
        }
    }

    return status;
}

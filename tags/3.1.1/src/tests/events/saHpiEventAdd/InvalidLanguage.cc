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

#include "InvalidLanguage.h"
#include "TextBufferHelper.h"
#include "EventHelper.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidLanguage::InvalidLanguage(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidLanguage::getName() {
    return "InvalidLanguage";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidLanguage::getDescription() {
    return "The <i>Language</i> is not one of the enumerated values for\n"
           "that type when the <i>DataType</i> is SAHPI_TL_TYPE_UNICODE\n"
           "or SAHPI_TL_TYPE_TEXT.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidLanguage::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidLanguage::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;
    SaHpiTextBufferT buf;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE,
                                  SAHPI_TL_TYPE_TEXT };
    int invalidLanguage = (int) SAHPI_LANG_ZULU + 1;

    for (int i = 0; i < 2; i++) {
        TextBufferHelper::fillByDataType(&buf, dataType[i]);
        EventHelper::fill(&event, &buf);
        event.EventDataUnion.UserEvent.UserEventData.Language = 
                                       (SaHpiLanguageT) invalidLanguage;
        SaErrorT error = saHpiEventAdd(sessionId, &event);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, EVENT_ADD, SA_ERR_HPI_INVALID_PARAMS, error,
                                 "DataType is %s", HpiString::dataType(dataType[i]));
        }
    }

    return status;
}

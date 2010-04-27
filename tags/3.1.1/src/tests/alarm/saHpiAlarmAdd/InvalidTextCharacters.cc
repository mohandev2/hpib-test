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
 *     Akma A. Muqeeth <aax6@unh.edu>
 *     Lars Wetzel <Lars.Wetzel@emerson.com>
 */

#include "AlarmHelper.h"
#include "TextBufferHelper.h"
#include "InvalidTextCharacters.h"

using namespace ns_saHpiAlarmAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidTextCharacters::InvalidTextCharacters(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getName() {
    return "InvalidTextCharacters";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getDescription() {
 return "The SaHpiTextBufferT structure passed as\n"
           "<i>Alarm->AlarmCond.Data</i> contains characters\n"
           "that are not allowed according to the value of\n"
           "<i>Alarm->AlarmCond.Data.DataType</i>. The tested\n"
           "types are SAHPI_TL_TYPE_UNICODE, SAHPI_TL_TYPE_BCDPLUS, and\n"
           "SAHPI_TL_TYPE_ASCII6.";

}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidTextCharacters::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *Add an alarm and then try to set it's TextBuffer to something invalid.
 *****************************************************************************/

HpiTestStatus InvalidTextCharacters::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE,
                                  SAHPI_TL_TYPE_BCDPLUS,
                                  SAHPI_TL_TYPE_ASCII6 };

    AlarmHelper::fill(&newAlarm);

    status.assertNotSupport();
    for (int i = 0; i < 3; i++) {
      TextBufferHelper::fillInvalidData(&newAlarm.AlarmCond.Data, dataType[i]);
      SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);

      if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
      } else {
        status.assertFailure(TRACE, ALARM_ADD, SA_ERR_HPI_INVALID_PARAMS, error, 
			     "Failed for DataTpe %s", HpiString::dataType(dataType[i]));
      }
    }
      
    return status;
}

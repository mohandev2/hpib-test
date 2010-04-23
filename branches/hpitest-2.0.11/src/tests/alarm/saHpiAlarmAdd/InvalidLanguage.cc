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
 *     Akmal A. Muqeeth <aax6@unh.edu>
 */

#include "InvalidLanguage.h"
#include "AlarmHelper.h"

using namespace ns_saHpiAlarmAdd;

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
    return "The <i>Alarm->AlarmCond.Type</i> is not SAHPI_STATUS_COND_TYPE_USER.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidLanguage::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *Add an alarm and then try to set it's DataType to something invalid.
 *****************************************************************************/

HpiTestStatus InvalidLanguage::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiAlarmT newAlarm;
 
   int invalidLanguage = (int) SAHPI_LANG_ZULU + 1;

       AlarmHelper::fill(&newAlarm);
        
       newAlarm.AlarmCond.Data.Language = (SaHpiLanguageT) invalidLanguage;

        SaErrorT error = saHpiAlarmAdd(sessionId, &newAlarm);
        if (error == SA_ERR_HPI_INVALID_PARAMS) 
        {
            status.assertPass();
        } else 
        {
            status.assertFailure(TRACE, ALARM_ADD,
                                 SA_ERR_HPI_INVALID_PARAMS, error );
        }
    

    return status;
}

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

#include "NullAlarm.h"

using namespace ns_saHpiAlarmGetNext;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullAlarm::NullAlarm(char *line) : NonEmptyAlarmTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullAlarm::getName() {
    return "NullAlarm";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullAlarm::getDescription() {
    return "The <i>Alarm</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullAlarm::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullAlarm::runAlarmTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;

    SaErrorT error = saHpiAlarmGetNext(sessionId, SAHPI_MINOR,
                                       SAHPI_FALSE, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, ALARM_GET_NEXT,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

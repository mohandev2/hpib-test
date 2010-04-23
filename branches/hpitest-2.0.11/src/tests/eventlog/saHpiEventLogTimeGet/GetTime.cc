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

#include "GetTime.h"

using namespace ns_saHpiEventLogTimeGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetTime::GetTime(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetTime::getName() {
    return "GetTime";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetTime::getDescription() {
    return "Get the time and verify that it is not SAHPI_TIME_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetTime::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetTime::runLogTest(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiTimeT time;

    SaErrorT error = saHpiEventLogTimeGet(sessionId, resourceId, &time);
    if (error != SA_OK) {
        status.assertFailure(TRACE, EVENT_LOG_TIME_GET, SA_OK, error);
    } else if (time == SAHPI_TIME_UNSPECIFIED) {
        status.assertFailure(TRACE, "saHpiEventLogTimeGet() returned a time "
                                    "of SAHPI_TIME_UNSPECIFIED.");
    } else {
        status.assertPass();
    }

    return status;
}


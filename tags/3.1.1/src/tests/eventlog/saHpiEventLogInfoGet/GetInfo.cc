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

#include "GetInfo.h"

using namespace ns_saHpiEventLogInfoGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetInfo::GetInfo(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetInfo::getName() {
    return "GetInfo";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetInfo::getDescription() {
    return "Retrieve the Event Log Info for the domain and the\n"
           "resources that support Event Logs.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetInfo::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetInfo::runLogTest(SaHpiSessionIdT sessionId,
                                  SaHpiResourceIdT resourceId) {
    HpiTestStatus status;
    SaHpiEventLogInfoT info;

    SaErrorT error = saHpiEventLogInfoGet(sessionId, resourceId, &info);
    if (error == SA_OK) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_INFO_GET, SA_OK, error);
    } 

    return status;
}


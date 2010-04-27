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

#include "InvalidTime.h"

using namespace ns_saHpiEventLogTimeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidTime::InvalidTime(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidTime::getName() {
    return "InvalidTime";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidTime::getDescription() {
    return "Set the <i>Time</i> parameter to SAHPI_TIME_UNSPECIFIED.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidTime::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidTime::runLogTest(SaHpiSessionIdT sessionId,
                                      SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    SaErrorT error = saHpiEventLogTimeSet(sessionId,
                                          resourceId,
                                          SAHPI_TIME_UNSPECIFIED);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_TIME_SET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


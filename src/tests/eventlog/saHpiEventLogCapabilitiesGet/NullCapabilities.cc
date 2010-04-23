/*      
 * (C) Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Suntrupth S Yadav <suntrupth@in.ibm.com>
 */

#include "NullCapabilities.h"

using namespace ns_saHpiEventLogCapabilitiesGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullCapabilities::NullCapabilities(char *line) : EventLogTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullCapabilities::getName() {
    return "NullCapabilities";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullCapabilities::getDescription() {
    return "The <i>Capabilities</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullCapabilities::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullCapabilities::runLogTest(SaHpiSessionIdT sessionId,
                                   SaHpiResourceIdT resourceId) {
    HpiTestStatus status;

    SaErrorT error = saHpiEventLogCapabilitiesGet(sessionId, resourceId, NULL);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_LOG_CAPABILITIES_GET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


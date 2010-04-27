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

#include "NoSubscription.h"

using namespace ns_saHpiEventGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoSubscription::NoSubscription(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoSubscription::getName() {
    return "NoSubscription";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoSubscription::getDescription() {
    return "The session is not currently subscribed for events.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoSubscription::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_REQUEST;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoSubscription::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;

    // Make sure that there really is no subscription
    // Ignore any errors since we might not have any current subscription.
    saHpiUnsubscribe(sessionId);

    SaErrorT error = saHpiEventGet(sessionId, 
                                   SAHPI_TIMEOUT_IMMEDIATE, 
                                   &event, NULL, NULL, NULL);
    if (error == SA_ERR_HPI_INVALID_REQUEST) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_GET, SA_ERR_HPI_INVALID_REQUEST, error);
    }

    return status;
}


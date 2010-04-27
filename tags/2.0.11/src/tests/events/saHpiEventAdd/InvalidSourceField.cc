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

#include "InvalidSourceField.h"
#include "EventHelper.h"

using namespace ns_saHpiEventAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSourceField::InvalidSourceField(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidSourceField::getName() {
    return "InvalidSourceField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidSourceField::getDescription() {
    return "The event type is SAHPI_ET_USER but the <i>Source</i> field\n"
           "is not SAHPI_UNSPECIFIED_RESOURCE_ID.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidSourceField::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSourceField::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEventT event;

    EventHelper::fill(&event);
    event.Source = getValidResourceId();
    SaErrorT error = saHpiEventAdd(sessionId, &event);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, EVENT_ADD, SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;
}

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

#include "InvalidDomainId.h"

using namespace ns_saHpiSessionOpen;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidDomainId::InvalidDomainId(char *line) : HpiGenTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidDomainId::getName() {
    return "InvalidDomainId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidDomainId::getDescription() {
    return "Test a domain identifier for which there is no matching domain.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidDomainId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DOMAIN;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidDomainId::run() {
    HpiTestStatus status;
    SaHpiSessionIdT sessionId;

    SaErrorT error = saHpiSessionOpen(getInvalidDomainId(), &sessionId, NULL);
    if (error == SA_ERR_HPI_INVALID_DOMAIN) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, SESSION_OPEN, 
                             SA_ERR_HPI_INVALID_DOMAIN, error);
    } 

    return status;
}


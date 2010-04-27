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

#include "NullRdr.h"

using namespace ns_saHpiRdrGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullRdr::NullRdr(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullRdr::getName() {
    return "NullRdr";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullRdr::getDescription() {
    return "The <i>Rdr</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NullRdr::getPrecondition() {
    return "Requires one or more resources with the RDR capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullRdr::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullRdr::runResourceTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasRdrCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaHpiEntryIdT nextEntryId;
        SaErrorT error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                     SAHPI_FIRST_ENTRY, &nextEntryId, NULL);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RDR_GET,
                                 SA_ERR_HPI_INVALID_PARAMS, error);
        }
    }

    return status;
}

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

#include "InvalidEntryId.h"

using namespace ns_saHpiRdrGet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidEntryId::InvalidEntryId(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidEntryId::getName() {
    return "InvalidEntryId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidEntryId::getDescription() {
    return "The <i>EntryId</i> that is passed does not correspond to an\n"
           "actual <i>EntryId</i> in the resource's RDR repository.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidEntryId::getPrecondition() {
    return "Requires one or more resources with the RDR capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidEntryId::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidEntryId::runResourceTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasRdrCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaHpiRdrT rdr;
        SaHpiEntryIdT nextEntryId;
        SaErrorT error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                     getInvalidEntryId(), &nextEntryId, &rdr);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RDR_GET,
                                 SA_ERR_HPI_NOT_PRESENT, error);
        }
    }

    return status;
}


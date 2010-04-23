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

#include "NoRdrCapability.h"

using namespace ns_saHpiRdrGetByInstrumentId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoRdrCapability::NoRdrCapability(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoRdrCapability::getName() {
    return "NoRdrCapability";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoRdrCapability::getDescription() {
    return "The resource contains no RDR records (and thus does not\n"
           "have the SAHPI_CAPABILITY_RDR flag set in its RPT entry).";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoRdrCapability::getPrecondition() {
    return "Requires one or more resources without the RDR capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoRdrCapability::getExpectedReturn() {
    return SA_ERR_HPI_CAPABILITY;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NoRdrCapability::runResourceTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (hasRdrCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaHpiRdrT rdr;
        SaErrorT error = saHpiRdrGetByInstrumentId(sessionId,
                                                   rptEntry->ResourceId,
                                                   getValidRdrType(),
                                                   getValidInstrumentId(), &rdr);
        if (error == SA_ERR_HPI_CAPABILITY) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RDR_GET, SA_ERR_HPI_CAPABILITY, error);
        }
    }

    return status;
}


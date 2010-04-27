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

#include "AnnunciatorNotPresent.h"

using namespace ns_saHpiAnnunciatorModeSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AnnunciatorNotPresent::AnnunciatorNotPresent(char *line)
: HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *AnnunciatorNotPresent::getName() {
    return "AnnunciatorNotPresent";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *AnnunciatorNotPresent::getDescription() {
    return "The <i>AnnunciatorNum</i> passed does not address a valid\n"
           "Annunciator supported by the resource.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *AnnunciatorNotPresent::getPrecondition() {
    return "Requires a resource that supports Annunciators.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT AnnunciatorNotPresent::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus AnnunciatorNotPresent::runResourceTest(SaHpiSessionIdT sessionId,
                                                     SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;

    if (!hasAnnunciatorCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        SaErrorT error = saHpiAnnunciatorModeSet(sessionId, resourceId, 
                                                 getInvalidAnnunciatorNum(), 
                                                 SAHPI_ANNUNCIATOR_MODE_USER);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, ANNUNCIATOR_MODE_SET,
                                 SA_ERR_HPI_NOT_PRESENT, error);
        }
    }

    return status;
}

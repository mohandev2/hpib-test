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

#include "HpiTestStatus.h"
#include "HpiRdrTestCase.h"
#include "LocationMgr.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiRdrTestCase::HpiRdrTestCase(SaHpiCapabilitiesT c)  {
    this->capability = c;
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

HpiRdrTestCase::HpiRdrTestCase(char *line, SaHpiCapabilitiesT c) 
: HpiResourceTestCase(line) { 
    this->capability = c;
}

/*****************************************************************************
 * Execute an HPI RDR Test Case.
 *
 * This involves traversing each session, each resource, and each rdr.
 *****************************************************************************/

HpiTestStatus HpiRdrTestCase::runResourceTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiRdrT rdr;

    status.assertNotSupport();
    if ((rptEntry->ResourceCapabilities & SAHPI_CAPABILITY_RDR) &&
        (rptEntry->ResourceCapabilities & capability)) {

        SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
        while (!status.hasFault() && nextEntryId != SAHPI_LAST_ENTRY) {
            SaHpiEntryIdT entryId = nextEntryId;
            SaErrorT error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                         entryId, &nextEntryId, &rdr);
            if (error == SA_ERR_HPI_NOT_PRESENT) {
                break;
            } else if (error != SA_OK) {
                status.assertError(TRACE, RDR_GET, SA_OK, error);
                break;
            } else {
                LocationMgr::push("Rdr", rdr.RecordId);
                status.merge(runRdrTest(sessionId, rptEntry, &rdr));
                LocationMgr::pop();
            }
        }
    }

    return status;
}


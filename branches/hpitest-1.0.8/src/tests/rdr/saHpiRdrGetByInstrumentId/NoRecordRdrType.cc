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

#include "NoRecordRdrType.h"
#include "Report.h"
#include "RdrHelper.h"

using namespace ns_saHpiRdrGetByInstrumentId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoRecordRdrType::NoRecordRdrType(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoRecordRdrType::getName() {
    return "NoRecordRdrType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoRecordRdrType::getDescription() {
    return "The <i>RdrType</i> parameter is SAHPI_NO_RECORD.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoRecordRdrType::getPrecondition() {
    return "Requires one or more resources with the RDR capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoRecordRdrType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * Traverse the RDR Table to obtain valid Instrument Ids.
 *****************************************************************************/

HpiTestStatus NoRecordRdrType::runResourceTest(SaHpiSessionIdT sessionId,
                                               SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;

    if (!hasRdrCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        status.assertPass();
        Report report;
        SaHpiRdrT rdr1, rdr2;
        SaHpiEntryIdT nextEntryId = SAHPI_FIRST_ENTRY;
        while (nextEntryId != SAHPI_LAST_ENTRY) {
            SaHpiEntryIdT entryId = nextEntryId;
            SaErrorT error = saHpiRdrGet(sessionId, rptEntry->ResourceId,
                                         entryId, &nextEntryId, &rdr1);
            if (error != SA_OK) {
                status.assertError(TRACE, RDR_GET, SA_OK, error);
                break;
            } else if (rdr1.RdrType != SAHPI_NO_RECORD) {
                SaHpiInstrumentIdT id = RdrHelper::getId(&rdr1);
                error = saHpiRdrGetByInstrumentId(sessionId, rptEntry->ResourceId,
                                                  SAHPI_NO_RECORD, id, &rdr2);
                if (error != SA_ERR_HPI_INVALID_PARAMS) {
                    status.assertFailure(TRACE, RDR_GET_BY_INSTRUMENT_ID,
                                      SA_ERR_HPI_INVALID_PARAMS, error);
                    break;
                }
            }
        }
    }

    return status;
}


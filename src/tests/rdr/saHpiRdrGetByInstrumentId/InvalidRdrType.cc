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

#include "InvalidRdrType.h"
#include "Report.h"
#include "RdrHelper.h"

using namespace ns_saHpiRdrGetByInstrumentId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidRdrType::InvalidRdrType(char *line) : HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidRdrType::getName() {
    return "InvalidRdrType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidRdrType::getDescription() {
    return "The <i>RdrType</i> parameter is not a valid enumerated value "
           "for the type.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidRdrType::getPrecondition() {
    return "Requires one or more resources with the RDR capability.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidRdrType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 * Traverse the RDR table in order to obtain valid Instrument Ids.
 *****************************************************************************/

HpiTestStatus InvalidRdrType::runResourceTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    int invalidRdrType = (int) SAHPI_FUMI_RDR + 1;

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
                SaHpiRdrTypeT rdrType =  (SaHpiRdrTypeT) invalidRdrType;
                error = saHpiRdrGetByInstrumentId(sessionId, rptEntry->ResourceId,
                                                  rdrType, id, &rdr2);
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


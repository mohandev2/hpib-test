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

#include "NoInstrumentCapability.h"

using namespace ns_saHpiRdrGetByInstrumentId;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NoInstrumentCapability::NoInstrumentCapability(char *line)
: HpiResourceTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NoInstrumentCapability::getName() {
    return "NoInstrumentCapability";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NoInstrumentCapability::getDescription() {
    return "The type of management instrument specified in the <i>RdrType</i>\n"
           "parameter is not supported by the resource, as indicated by\n"
           "the <i>Capability</i> field in its RPT entry.";
}

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *NoInstrumentCapability::getPrecondition() {
        return "Requires one or more resources with the RDR capability,\n"
               "and NOT supporting one of the following capabilities:\n"
               "Control, Sensor, Inventory, Watchdog, and Annunciator.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NoInstrumentCapability::getExpectedReturn() {
    return SA_ERR_HPI_CAPABILITY;
}

/*****************************************************************************
 * Run the test.
 *
 * Test each management instrument not supported by the resource.
 *****************************************************************************/

HpiTestStatus NoInstrumentCapability::runResourceTest(SaHpiSessionIdT sessionId,
                                                      SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiCapabilitiesT capability[] = { SAHPI_CAPABILITY_CONTROL,
                                        SAHPI_CAPABILITY_SENSOR,
                                        SAHPI_CAPABILITY_INVENTORY_DATA,
                                        SAHPI_CAPABILITY_ANNUNCIATOR,
                                        SAHPI_CAPABILITY_WATCHDOG,
					SAHPI_CAPABILITY_DIMI,
					SAHPI_CAPABILITY_FUMI };

    SaHpiRdrTypeT rdrType[] = { SAHPI_CTRL_RDR,
                                SAHPI_SENSOR_RDR,
                                SAHPI_INVENTORY_RDR, 
                                SAHPI_ANNUNCIATOR_RDR,
                                SAHPI_WATCHDOG_RDR,
                                SAHPI_DIMI_RDR,
                                SAHPI_FUMI_RDR };

    if (!hasRdrCapability(rptEntry)) {
        status.assertNotSupport();
    } else {
        bool tested = false;
        for (int i = 0; i < 7; i++) {
            if (!hasCapability(rptEntry, capability[i])) {
                tested = true;
                status.add(TRACE, runBadRdrTest(sessionId,
                                                rptEntry->ResourceId,
                                                rdrType[i]));
            }
        }

        if (!tested) {
            status.assertNotSupport();
        }
    }

    return status;
}

/*****************************************************************************
 * Get an RDR using an RdrType that is not supported by the resource.
 *****************************************************************************/

HpiTestStatus NoInstrumentCapability::runBadRdrTest(SaHpiSessionIdT sessionId,
                                                    SaHpiResourceIdT resourceId, 
                                                    SaHpiRdrTypeT rdrType) {
    HpiTestStatus status;
    SaHpiRdrT rdr;

    SaErrorT error = saHpiRdrGetByInstrumentId(sessionId, resourceId, rdrType,
                                               getValidInstrumentId(), &rdr);
    if (error == SA_ERR_HPI_CAPABILITY) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RDR_GET_BY_INSTRUMENT_ID,
                             SA_ERR_HPI_CAPABILITY, error);
    }

    return status;
}


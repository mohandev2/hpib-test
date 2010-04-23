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

#include "IdrTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IdrTestCase::IdrTestCase()
: HpiRdrTestCase(SAHPI_CAPABILITY_INVENTORY_DATA) {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IdrTestCase::IdrTestCase(char *line)
: HpiRdrTestCase(line, SAHPI_CAPABILITY_INVENTORY_DATA) {
}

/*****************************************************************************
 * In order to run the actual test, we must be working with an
 * inventory data record.
 *****************************************************************************/

HpiTestStatus IdrTestCase::runRdrTest(SaHpiSessionIdT sessionId,
                                      SaHpiRptEntryT *rptEntry,
                                      SaHpiRdrT *rdr) {
    HpiTestStatus status;

    if (rdr->RdrType != SAHPI_INVENTORY_RDR) {
        status.assertNotSupport();
    } else {
        pushLocation("Idr", rdr->RdrTypeUnion.InventoryRec.IdrId);
        status = runIdrTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.InventoryRec));
        popLocation();
    }

    return status;
}


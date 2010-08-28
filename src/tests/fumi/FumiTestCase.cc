/*
 * (C) Copyright Lars Wetzel, 2009
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
 *     Lars Wetzel <larswetzel@users.sourceforge.net>
 */
 
#include "FumiTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FumiTestCase::FumiTestCase() : HpiRdrTestCase(SAHPI_CAPABILITY_FUMI) {
}


/*****************************************************************************
 * Constructor
 *****************************************************************************/

FumiTestCase::FumiTestCase(char *line) 
: HpiRdrTestCase(line, SAHPI_CAPABILITY_FUMI) { 
}

/*****************************************************************************
 * Test Fumi entries
 *****************************************************************************/

HpiTestStatus FumiTestCase::runRdrTest(SaHpiSessionIdT sessionId, 
                                    SaHpiRptEntryT *rptEntry, 
                                    SaHpiRdrT *rdr) {
    HpiTestStatus status;

    if (rdr->RdrType != SAHPI_FUMI_RDR) {
        status.assertNotSupport();
    } else {
        pushLocation("Fumi", rdr->RdrTypeUnion.FumiRec.Num);
        status = runFumiTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.FumiRec));
        popLocation();
    }

    return status;
}


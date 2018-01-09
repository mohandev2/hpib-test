/*      
 * (C) Copyright Emerson Corp. 2009
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 *  Authors:
 *  Lars Wetzel   <Lars.Wetzel@emerson.com>
 */

#include "InvalidSession.h"
#include "ResourceHelper.h"

using namespace ns_saHpiGetIdByEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSession::InvalidSession(char *line) : HpiInvalidSessionTestCase(line) {
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSession::run() {
  HpiTestStatus status;
    SaHpiEntityPathT   entityPath;
    SaHpiResourceIdT   resourceId;
    SaHpiInstrumentIdT instrumentId;
    SaHpiUint32T       rptUpdateCount;

    ResourceHelper::fillEntityPath(&entityPath);
    SaErrorT error = saHpiGetIdByEntityPath(getInvalidSessionId(),entityPath, SAHPI_NO_RECORD,
					    SAHPI_FIRST_ENTRY, &resourceId, &instrumentId, &rptUpdateCount);
    
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, GET_ID_BY_ENTITY_PATH, 
                              SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}
	

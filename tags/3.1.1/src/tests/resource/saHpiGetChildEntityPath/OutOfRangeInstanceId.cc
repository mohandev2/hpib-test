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
 *  Anurag Saxena   <Anurag.Saxena@emerson.com>
 */
 
#include "OutOfRangeInstanceId.h"
#include "ResourceHelper.h"
 
using namespace ns_saHpiGetChildEntityPath;
 
/*****************************************************************************
 * Constructor
 *****************************************************************************/
 
OutOfRangeInstanceId::OutOfRangeInstanceId(char *line) : HpiSessionTestCase(line) {
}
 
/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/
 
const char *OutOfRangeInstanceId::getName() {
    return "OutOfRangeInstanceId";
}
 
/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/
 
const char *OutOfRangeInstanceId::getDescription() {
    return "A valid <i>InstanceId</> other that SAHPI_FIRST_ENTRY is passed \n"
           "as a value corresponding to which a entity has no child entity \n"
           " - like MAX(instanceIds returned by a parent entity) + 1";
}
 
/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/
 
SaErrorT OutOfRangeInstanceId::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}
 
/*****************************************************************************
 * Run the test.
 *****************************************************************************/
 
HpiTestStatus OutOfRangeInstanceId::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntityPathT  parentEntityPath;
    SaHpiUint32T      instanceId = SAHPI_FIRST_ENTRY;
    SaHpiEntityPathT  childEntityPath;
    SaHpiUint32T      rptUpdateCount;
    SaHpiUint32T      max = 0;
    ResourceHelper::fillEntityPath(&parentEntityPath);
 
    SaErrorT error = SA_OK;

    while (instanceId != SAHPI_LAST_ENTRY) {

      error = saHpiGetChildEntityPath(sessionId, parentEntityPath,
				      &instanceId, &childEntityPath, &rptUpdateCount );
      if (error != SA_OK) {
	status.assertFailure(TRACE, GET_CHILD_ENTITY_PATH, SA_OK, error,
			     "for parent entity path: %s\n"
			     "instanceId: %d\n",
			     HpiString::entityPath(&parentEntityPath),
			     instanceId);

	return status;
      }
      if((max < instanceId) &&
	 (instanceId != SAHPI_LAST_ENTRY))
	max = instanceId;
	
    }
      
    instanceId = max+1;

    if(instanceId != SAHPI_LAST_ENTRY) {
      error = saHpiGetChildEntityPath(sessionId, parentEntityPath,
				      &instanceId, &childEntityPath, &rptUpdateCount );
      
      if(error == SA_ERR_HPI_NOT_PRESENT) {
	status.assertPass();
      } else {
	status.assertFailure(TRACE, GET_CHILD_ENTITY_PATH,
			     SA_ERR_HPI_NOT_PRESENT, error);
      }
    } else {
      status.assertPass();
    }
    
    return status;

}

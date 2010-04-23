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

#include "GetChildEntityPath.h"
#include "ResourceHelper.h"

using namespace ns_saHpiGetChildEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

GetChildEntityPath::GetChildEntityPath(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *GetChildEntityPath::getName() {
    return "GetChildEntityPath";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *GetChildEntityPath::getDescription() {
    return "Get <i>ChildEntityPath</i> by passing valid <i>InstanceId</i> \n"
           "and <i>ParentEntityPath</i>";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT GetChildEntityPath::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus GetChildEntityPath::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntityPathT  parentEntityPath;

    status.assertPass();
    ResourceHelper::fillEntityPath(&parentEntityPath);
    
    status = parseDomainEntTree(sessionId, parentEntityPath);
    return status;
}

HpiTestStatus GetChildEntityPath::parseDomainEntTree(SaHpiSessionIdT sessionId, 
						     SaHpiEntityPathT parentEntityPath) {
  SaHpiUint32T instanceId = SAHPI_FIRST_ENTRY;
  SaErrorT error;
  HpiTestStatus status;
  SaHpiEntityPathT childEntityPath;
  SaHpiUint32T rptUpdateCount;
 
  while(instanceId != SAHPI_LAST_ENTRY) {
    error = saHpiGetChildEntityPath(sessionId, parentEntityPath,
				    &instanceId, &childEntityPath, &rptUpdateCount );

    if(error == SA_ERR_HPI_NOT_PRESENT && 
       instanceId == SAHPI_FIRST_ENTRY) {
      // We are in a leaf of the tree - there is no more child
      error = SA_OK;
      break;
      
    } else if(error != SA_OK) {
      status.assertFailure(TRACE, GET_CHILD_ENTITY_PATH,SA_OK, error,
			   "for parent entity path: %s\n"
			   "instanceId: %d\n",
			   HpiString::entityPath(&parentEntityPath),
			   instanceId);
      break;
    }
    
    status = parseDomainEntTree(sessionId, childEntityPath);
  }

  return status;
}

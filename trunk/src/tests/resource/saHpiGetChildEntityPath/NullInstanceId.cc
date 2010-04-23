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
 *  Anurag Saxena <Anurag.Saxena@emerson.com>
 */

#include "NullInstanceId.h"
#include "ResourceHelper.h"

using namespace ns_saHpiGetChildEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

NullInstanceId::NullInstanceId(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *NullInstanceId::getName() {
    return "NullInstanceId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *NullInstanceId::getDescription() {
    return "The <i>InstanceId</i> pointer is passed in as NULL.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT NullInstanceId::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus NullInstanceId::runSessionTest(SaHpiSessionIdT sessionId) {
  HpiTestStatus status;
  SaHpiEntityPathT  parentEntityPath;
  SaHpiEntityPathT  childEntityPath;
  SaHpiUint32T      rptUpdateCount;

  ResourceHelper::fillEntityPath(&parentEntityPath);

  SaErrorT error = saHpiGetChildEntityPath(sessionId, parentEntityPath, NULL,
					   &childEntityPath, &rptUpdateCount );

  if (error == SA_ERR_HPI_INVALID_PARAMS) {
    status.assertPass();
  } else {
    status.assertFailure(TRACE, GET_CHILD_ENTITY_PATH,
			 SA_ERR_HPI_INVALID_PARAMS, error);
  }
  
  return status;
}


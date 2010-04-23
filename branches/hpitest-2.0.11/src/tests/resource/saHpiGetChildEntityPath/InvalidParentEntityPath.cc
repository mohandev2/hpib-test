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

#include "InvalidParentEntityPath.h"
#include "ResourceHelper.h"

using namespace ns_saHpiGetChildEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidParentEntityPath::InvalidParentEntityPath(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidParentEntityPath::getName() {
    return "InvalidParentEntityPath";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidParentEntityPath::getDescription() {
    return "The <i>ParentEntityPath</i> passed is invalid";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidParentEntityPath::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_DATA;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidParentEntityPath::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntityPathT  parentEntityPath;
    SaHpiUint32T      instanceId;
    SaHpiEntityPathT  childEntityPath;
    SaHpiUint32T      rptUpdateCount;

    ResourceHelper::getInvalidEntityPath(&parentEntityPath);
    instanceId = SAHPI_FIRST_ENTRY;

    SaErrorT error = saHpiGetChildEntityPath(sessionId, parentEntityPath, &instanceId,
					     &childEntityPath, &rptUpdateCount );

    if (error == SA_ERR_HPI_INVALID_DATA) {
      status.assertPass();
    } else {
      status.assertFailure(TRACE, GET_CHILD_ENTITY_PATH,
			   SA_ERR_HPI_INVALID_DATA, error);
    }

    return status;
}


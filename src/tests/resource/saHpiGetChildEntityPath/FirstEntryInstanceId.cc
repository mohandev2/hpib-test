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

#include "FirstEntryInstanceId.h"
#include "ResourceHelper.h"

using namespace ns_saHpiGetChildEntityPath;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

FirstEntryInstanceId::FirstEntryInstanceId(char *line) : HpiSessionTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *FirstEntryInstanceId::getName() {
    return "FirstEntryInstanceId";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *FirstEntryInstanceId::getDescription() {
    return "SAHPI_FIRST_ENTRY is passed as <i>InstanceId</i> for an entity \n"
            "having no child entities.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT FirstEntryInstanceId::getExpectedReturn() {
    return SA_ERR_HPI_NOT_PRESENT;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus FirstEntryInstanceId::runSessionTest(SaHpiSessionIdT sessionId) {
    HpiTestStatus status;
    SaHpiEntityPathT  parentEntityPath;
    SaHpiUint32T      instanceId;
    SaHpiEntityPathT  childEntityPath;
    SaHpiUint32T      rptUpdateCount;

    ResourceHelper::fillEntityPath(&parentEntityPath);

    SaErrorT error;

    SaHpiInt32T depth = 0;

    while(depth < MAX_DOMAIN_ENTITY_TREE_DEPTH)
    {
        instanceId = SAHPI_FIRST_ENTRY;
        error = saHpiGetChildEntityPath(sessionId, parentEntityPath,
                   &instanceId, &childEntityPath, &rptUpdateCount );

        if(error == SA_ERR_HPI_NOT_PRESENT)
        {
             status.assertPass();
             break;
        }
        else if(error != SA_OK)
        {
            status.assertFailure(TRACE, GET_CHILD_ENTITY_PATH,
                             SA_ERR_HPI_NOT_PRESENT, error,
			      "Function was called %d times\n", (depth +1));
            break;
        }
        
        parentEntityPath = childEntityPath;
        depth++;
    }

    if (depth == MAX_DOMAIN_ENTITY_TREE_DEPTH)
    { 
      status.assertError("It looks like the tree has a cycle or it too deep\n");
    }

    return status;
}


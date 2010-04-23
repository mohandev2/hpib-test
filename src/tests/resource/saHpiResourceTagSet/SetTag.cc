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

#include "SetTag.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiResourceTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

SetTag::SetTag(char *line) : TagSetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *SetTag::getName() {
    return "SetTag";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *SetTag::getDescription() {
    return "Set the resource tag and verify that the set was successful.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT SetTag::getExpectedReturn() {
    return SA_OK;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus SetTag::runTagTest(SaHpiSessionIdT sessionId,
                                 SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiTextBufferT tag;
    SaHpiRptEntryT newRptEntry;

    TextBufferHelper::fill(&tag);
    SaErrorT error = saHpiResourceTagSet(sessionId, rptEntry->ResourceId, &tag);
    if (error != SA_OK) {
        status.assertFailure(TRACE, RESOURCE_TAG_SET, SA_OK, error);
    } else {
        error = saHpiRptEntryGetByResourceId(sessionId, rptEntry->ResourceId, &newRptEntry);
        if (error != SA_OK) {
            status.assertError(TRACE, RPT_ENTRY_GET_BY_RESOURCE_ID, SA_OK, error);
        } else if (!TextBufferHelper::isEqual(&tag, &newRptEntry.ResourceTag)) {
            status.assertFailure(TRACE, "The resource tag was not actually changed!");
        } else {
            status.assertPass();
        }
    }

    return status;
}


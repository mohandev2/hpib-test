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

#include "InvalidDataType.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiResourceTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidDataType::InvalidDataType(char *line) : TagSetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidDataType::getName() {
    return "InvalidDataType";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidDataType::getDescription() {
    return "The <i>DataType</i> is not one of the valid enumerated values "
           "for that type.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidDataType::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidDataType::runTagTest(SaHpiSessionIdT sessionId,
                                          SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiTextBufferT tag;
    int invalidDataType = (int) SAHPI_TL_TYPE_BINARY + 1;

    TextBufferHelper::fill(&tag);
    tag.DataType = (SaHpiTextTypeT) invalidDataType;
    SaErrorT error = saHpiResourceTagSet(sessionId, rptEntry->ResourceId, &tag);
    if (error == SA_ERR_HPI_INVALID_PARAMS) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, RESOURCE_TAG_SET,
                             SA_ERR_HPI_INVALID_PARAMS, error);
    } 

    return status;
}


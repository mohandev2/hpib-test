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

#include "InvalidDataField.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiResourceTagSet;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidDataField::InvalidDataField(char *line) : TagSetTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidDataField::getName() {
    return "InvalidDataField";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidDataField::getDescription() {
    return "The <i>Data</i> field contains characters that are not legal\n"
           "according to the value of <i>DataType</i>.\n"
           "The tested DataTypes are UNICODE, BCDPLUS, and ASCII6.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidDataField::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidDataField::runTagTest(SaHpiSessionIdT sessionId,
                                           SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiTextBufferT tag;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE,
                                  SAHPI_TL_TYPE_BCDPLUS,
                                  SAHPI_TL_TYPE_ASCII6 };

    for (int i = 0; i < 3; i++) {
        TextBufferHelper::fillInvalidData(&tag, dataType[i]);

        SaErrorT error = saHpiResourceTagSet(sessionId, rptEntry->ResourceId, &tag);
        if (error == SA_ERR_HPI_INVALID_PARAMS) {
            status.assertPass();
        } else {
            status.assertFailure(TRACE, RESOURCE_TAG_SET,
                                 SA_ERR_HPI_INVALID_PARAMS, error,
                                 "Text Buffer is %s", HpiString::textBuffer(tag));
        }
    }

    return status;
}

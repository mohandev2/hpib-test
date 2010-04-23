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
 *     Akmal Muqeeth <akmal.muqeeth@unh.edu>
 *     Lars Wetzel <Lars.Wetzel@emerson.com>
 */

#include "InvalidTextCharacters.h"
#include "AnnunciatorHelper.h"
#include "TextBufferHelper.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidTextCharacters::InvalidTextCharacters(char *line) : AnnunciatorTestCase(line) {
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getName() {
    return "InvalidTextCharacters";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getDescription() {
    return "The SaHpiTextBufferT structure passed as\n"
           "<i> announcement.StatusCond.Data</i> must not contain text\n"
           "characters that are not allowed according to the value\n"
           "of <i> announcement.StatusCond.Data.DataType</i>.";
    }

/*****************************************************************************
 * Return the precondition of the test case.
 *****************************************************************************/

const char *InvalidTextCharacters::getPrecondition() {
    return "Requires an Annunciator for which announcements can be added.";
}

/*****************************************************************************
 * Return the expected error code.
 *****************************************************************************/

SaErrorT InvalidTextCharacters::getExpectedReturn() {
    return SA_ERR_HPI_INVALID_PARAMS;
}

/*****************************************************************************
 * Run the test.
 *
 *****************************************************************************/

HpiTestStatus InvalidTextCharacters::runAnnunTest(SaHpiSessionIdT sessionId,
                                            SaHpiRptEntryT *rptEntry,
                                            SaHpiAnnunciatorRecT *annunRec) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;
    SaHpiAnnouncementT announcement;
    SaHpiTextTypeT dataType[] = { SAHPI_TL_TYPE_UNICODE,
                                  SAHPI_TL_TYPE_BCDPLUS,
                                  SAHPI_TL_TYPE_ASCII6 };

    status.add(TRACE, setWriteMode(sessionId, resourceId, annunRec));

    if (status.isOkay()) {
      
      AnnunciatorHelper::fill(&announcement);
            
      status.assertNotSupport();
      for (int i = 0; i < 3; i++) {
	TextBufferHelper::fillInvalidData(&announcement.StatusCond.Data, dataType[i]);
	SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
					     annunNum, &announcement);

	if (error == SA_ERR_HPI_INVALID_PARAMS) {
	  status.assertPass();
	} else {
	  status.assertFailure(TRACE, ANNUNCIATOR_ADD, SA_ERR_HPI_INVALID_PARAMS, error,
			       "Failed for DataTpe %s", HpiString::dataType(dataType[i]));
	} 
      }
      
      status.add(TRACE, restoreMode(sessionId, resourceId, annunNum));
    }

    return status;
}

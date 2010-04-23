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
 *     Akmal A. Muqeeth <aax6@unh.edu>
 */

#include "LastEntry.h"
#include "AnnunciatorHelper.h"
using namespace ns_saHpiAnnunciatorGet;


/*****************************************************************************
 * Constructor
 *****************************************************************************/

LastEntry::LastEntry(char *line) : AnnunciatorTestCase(line) {
    errorList.add(SA_OK);
    errorList.add(SA_ERR_HPI_NOT_PRESENT);
}

/*****************************************************************************
 * Return the name of the test case.
 *****************************************************************************/

const char *LastEntry::getName() {
    return "LastEntry";
}

/*****************************************************************************
 * Return the description of the test case.
 *****************************************************************************/

const char *LastEntry::getDescription() {
    return "Use SAHPI_LAST_ENTRY as the <i>EntryId</i> to obtain the last\n"
           "announcement in the Annnunciator's current set. ";
}

/*****************************************************************************
 * Return the expected error codes.
 *****************************************************************************/

ErrorList *LastEntry::getExpectedReturnList() {
    return &errorList;
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus LastEntry::runResourceTest(SaHpiSessionIdT sessionId,
                                                     SaHpiRptEntryT *rptEntry) {
    HpiTestStatus status;
    SaHpiResourceIdT resourceId = rptEntry->ResourceId;
    SaHpiAnnouncementT announcement;
    
    status.assertPass();

   SaErrorT error = saHpiAnnunciatorGet(sessionId, resourceId,
                                             getValidAnnunciatorNum(),
                                              SAHPI_LAST_ENTRY,
                                             &announcement);

if (error != SA_ERR_HPI_INVALID_PARAMS) 
    {
        status.assertFailure(TRACE, ANNUNCIATOR_GET, SA_ERR_HPI_INVALID_PARAMS, error);
    }

    return status;

}

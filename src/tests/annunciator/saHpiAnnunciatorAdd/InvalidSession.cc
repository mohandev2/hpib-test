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

#include "InvalidSession.h"
#include "AnnunciatorHelper.h"

using namespace ns_saHpiAnnunciatorAdd;

/*****************************************************************************
 * Constructor
 *****************************************************************************/

InvalidSession::InvalidSession(char *line) : HpiInvalidSessionTestCase(line) {
}

/*****************************************************************************
 * Run the test.
 *****************************************************************************/

HpiTestStatus InvalidSession::run() {
    HpiTestStatus status;
    SaHpiAnnouncementT announcement;

    AnnunciatorHelper::fill(&announcement);
    SaErrorT error = saHpiAnnunciatorAdd(getInvalidSessionId(),
                                         getValidResourceId(), 
                                         getValidAnnunciatorNum(),
                                         &announcement);

    if (error == SA_ERR_HPI_INVALID_SESSION) {
        status.assertPass();
    } else {
        status.assertFailure(TRACE, ANNUNCIATOR_ADD,
                             SA_ERR_HPI_INVALID_SESSION, error);
    } 

    return status;
}


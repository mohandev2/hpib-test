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

#ifndef __NOSEVERITYUNACKANNOUNCEMENTS_H__
#define __NOSEVERITYUNACKANNOUNCEMENTS_H__

#include "AnnunciatorTestCase.h"

/*****************************************************************************
 * No Severity Unack Announcements Test Case
 *****************************************************************************/

namespace ns_saHpiAnnunciatorGetNext
{
    class NoSeverityUnackAnnouncements : public AnnunciatorTestCase
    {
    public:
        NoSeverityUnackAnnouncements(char *line);

        HpiTestStatus runAnnunTest(SaHpiSessionIdT sessionId, 
                                   SaHpiRptEntryT *rptEntry, 
                                   SaHpiAnnunciatorRecT *annunRec);

        const char *getName();

        const char *getDescription();

        SaErrorT getExpectedReturn();

    private:
        HpiTestStatus runNotPresentTest(SaHpiSessionIdT sessionId,
                                        SaHpiResourceIdT resourceId,
                                        SaHpiAnnunciatorNumT annunNum,
                                        SaHpiSeverityT severity);

        HpiTestStatus prepAndRunTest(SaHpiSessionIdT sessionId,
                                     SaHpiResourceIdT resourceId,
                                     SaHpiAnnunciatorRecT *annunRec);
    };
}

#endif

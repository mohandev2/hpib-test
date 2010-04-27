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

#ifndef __ANNUNCIATORTESTCASE_H___
#define __ANNUNCIATORTESTCASE_H___

#include "SafTest.h"
#include "HpiRdrTestCase.h"
#include "AnnouncementSet.h"
#include "AnnunciatorHelper.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/

#define MAX_SEVERITIES 6  // number of valid severities

/*****************************************************************************
 * Base Annunciator Test Case
 *****************************************************************************/

class AnnunciatorTestCase : public HpiRdrTestCase
{
private:
    SaHpiAnnunciatorModeT annunMode;
    AnnouncementSet ackAnnouncementSet;
    AnnouncementSet unackAnnouncementSet;

public:
    AnnunciatorTestCase();
    AnnunciatorTestCase(char* line);

protected:
    HpiTestStatus runRdrTest(SaHpiSessionIdT sessionId,
                             SaHpiRptEntryT *rptEntry,
                             SaHpiRdrT *rdr);

    virtual HpiTestStatus runAnnunTest(SaHpiSessionIdT sessionId,
                                       SaHpiRptEntryT *rptEntry,
                                       SaHpiAnnunciatorRecT *annunRec) = 0;

    HpiTestStatus setWriteMode(SaHpiSessionIdT         sessionId,
                               SaHpiResourceIdT        resourceId,
                               SaHpiAnnunciatorRecT    *annunRec);

    HpiTestStatus restoreMode(SaHpiSessionIdT         sessionId,
                              SaHpiResourceIdT        resourceId,
                              SaHpiAnnunciatorNumT    annunNum);

    HpiTestStatus addTestAnnouncements(SaHpiSessionIdT       sessionId,
                                       SaHpiResourceIdT      resourceId,
                                       SaHpiAnnunciatorNumT  annunNum);

    HpiTestStatus deleteTestAnnouncements(SaHpiSessionIdT       sessionId,
                                          SaHpiResourceIdT      resourceId,
                                          SaHpiAnnunciatorNumT  annunNum);

    HpiTestStatus addSeverityAnnouncements(SaHpiSessionIdT       sessionId,
                                           SaHpiResourceIdT      resourceId,
                                           SaHpiAnnunciatorNumT  annunNum,
                                           AnnouncementSet       *announcementSet);

    HpiTestStatus deleteAnnouncements(SaHpiSessionIdT       sessionId,
                                      SaHpiResourceIdT      resourceId,
                                      SaHpiAnnunciatorNumT  annunNum,
                                      AnnouncementSet       *announcementSet);

    HpiTestStatus ackAnnouncements(SaHpiSessionIdT       sessionId,
                                   SaHpiResourceIdT      resourceId,
                                   SaHpiAnnunciatorNumT  annunNum,
                                   AnnouncementSet       *announcementSet);

    SaHpiSeverityT *getValidSeverities(int *count);

    HpiTestStatus getAnnouncementCount(SaHpiSessionIdT        sessionId,
                                       SaHpiResourceIdT       resourceId,
                                       SaHpiAnnunciatorNumT   annunNum,
                                       SaHpiSeverityT         severity,
                                       SaHpiBoolT             unacknowledgedOnly,
                                       int                    *count);

    bool hasSeverity(SaHpiAnnouncementT  *announcement,
                     SaHpiSeverityT severity);

    HpiTestStatus getUnusedSeverity(SaHpiSessionIdT        sessionId,
                                    SaHpiResourceIdT       resourceId,
                                    SaHpiAnnunciatorNumT   annunNum,
                                    SaHpiBoolT             unacknowledgedOnly,
                                    SaHpiSeverityT         *severity,
                                    bool                   *found);

    HpiTestStatus addAnnouncement(SaHpiSessionIdT        sessionId,
                                  SaHpiResourceIdT       resourceId,
                                  SaHpiAnnunciatorNumT   annunNum,
                                  SaHpiSeverityT         severity,
                                  SaHpiAnnouncementT     *announcement);

    HpiTestStatus addInfoAnnouncement(SaHpiSessionIdT        sessionId,
                                      SaHpiResourceIdT       resourceId,
                                      SaHpiAnnunciatorNumT   annunNum,
                                      SaHpiAnnouncementT     *announcement);

    HpiTestStatus deleteAnnouncement(SaHpiSessionIdT        sessionId,
                                     SaHpiResourceIdT       resourceId,
                                     SaHpiAnnunciatorNumT   annunNum,
                                     SaHpiAnnouncementT     *announcement);

    HpiTestStatus getUnusedSeverities(SaHpiSessionIdT        sessionId,
                                      SaHpiResourceIdT       resourceId,
                                      SaHpiAnnunciatorNumT   a_num,
                                      SaHpiBoolT             unacknowledgedOnly,
                                      SaHpiSeverityT         severity[],
                                      int                    *scount);

    HpiTestStatus containsAnnouncement(SaHpiSessionIdT        sessionId,
                                       SaHpiResourceIdT       resourceId,
                                       SaHpiAnnunciatorNumT   annunNum,
                                       SaHpiEntryIdT          entryId,
                                       bool                   *found);

    HpiTestStatus isEmpty(SaHpiSessionIdT        sessionId,
                          SaHpiResourceIdT       resourceId,
                          SaHpiAnnunciatorNumT   a_num,
                          bool                   *empty);
};

#endif


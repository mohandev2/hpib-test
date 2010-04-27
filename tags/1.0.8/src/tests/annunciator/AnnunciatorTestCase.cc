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

#include "AnnunciatorTestCase.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AnnunciatorTestCase::AnnunciatorTestCase()
: HpiRdrTestCase(SAHPI_CAPABILITY_ANNUNCIATOR) {
}

/*****************************************************************************
 * Constructor
 *****************************************************************************/

AnnunciatorTestCase::AnnunciatorTestCase(char *line)
: HpiRdrTestCase(line, SAHPI_CAPABILITY_ANNUNCIATOR) {
}

/*****************************************************************************
 * In order to run the main test, this RDR must be an Annunciator.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::runRdrTest(SaHpiSessionIdT sessionId,
                                              SaHpiRptEntryT *rptEntry,
                                              SaHpiRdrT *rdr) {
    HpiTestStatus status;

    if (!hasAnnunciatorCapability(rptEntry)) {
        status.assertNotSupport();
    } else if (rdr->RdrType != SAHPI_ANNUNCIATOR_RDR) {
        status.assertNotSupport();
    } else {
        pushLocation("Annunciator", rdr->RdrTypeUnion.AnnunciatorRec.AnnunciatorNum);

        ackAnnouncementSet.clear();
        unackAnnouncementSet.clear();

        status = runAnnunTest(sessionId, rptEntry, &(rdr->RdrTypeUnion.AnnunciatorRec));
        popLocation();
    }

    return status;
}

/*****************************************************************************
 * The setWriteMode() function attempts to place the Annunciator
 * into either USER or SHARED mode.  If * the Annunciator is already in the USER
 * or SHARED mode, then this method does nothing.  If the Annunciator is in AUTO
 * mode, an attempt is made to change to SHARED mode.
 *
 * Return values:
 *     SA_OK: Annunciator is in a write mode (USER or SHARED).
 *     SA_ERR_HPI_READ_ONLY: Annunciator is in read-only mode and cannot be changed.
 *     SA_ERR_HPI_ERROR: an unexpected error occured with an HPI function invocation.
 *
 * Upon success (SA_OK), the "mode" contains the original mode which
 * can be later restored with the restoreMode() function.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::setWriteMode(SaHpiSessionIdT         sessionId,
                                                SaHpiResourceIdT        resourceId,
                                                SaHpiAnnunciatorRecT    *annunRec) {
    HpiTestStatus status;
    SaHpiAnnunciatorNumT annunNum = annunRec->AnnunciatorNum;

    SaErrorT error = saHpiAnnunciatorModeGet(sessionId, resourceId, annunNum, &annunMode);
    if (error != SA_OK) {
        status.assertError(TRACE, ANNUNCIATOR_MODE_GET, SA_OK, error);
    } else if (annunMode == SAHPI_ANNUNCIATOR_MODE_AUTO) {
        if (annunRec->ModeReadOnly) {
            status.assertNotSupport();
        } else {
            error = saHpiAnnunciatorModeSet(sessionId, resourceId, annunNum,
                                            SAHPI_ANNUNCIATOR_MODE_SHARED);
            if (error != SA_OK) {
                status.assertError(TRACE, ANNUNCIATOR_MODE_SET, SA_OK, error);
            }
        }
    }

    return status;
}

/*****************************************************************************
 * The restoreMode() function works in conjunction with the setWriteMode()
 * function.  It actually only restores the mode if the mode was changed from
 * AUTO to SHARED by the setWriteMode() function.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::restoreMode(SaHpiSessionIdT        sessionId,
                                               SaHpiResourceIdT       resourceId,
                                               SaHpiAnnunciatorNumT   annunNum) {
    HpiTestStatus status;

    if (annunMode == SAHPI_ANNUNCIATOR_MODE_AUTO) {
        SaErrorT error = saHpiAnnunciatorModeSet(sessionId, resourceId, annunNum, annunMode);
        if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_MODE_SET, SA_OK, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Add a bunch of announcements for test purposes.  There are two sets, one
 * that is unacknowledged and the other is acknowledged.  Each has one announcement
 * for each severity level.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::addTestAnnouncements(SaHpiSessionIdT       sessionId,
                                                        SaHpiResourceIdT      resourceId,
                                                        SaHpiAnnunciatorNumT  annunNum) {
    HpiTestStatus  status;

    status.add(TRACE, addSeverityAnnouncements(sessionId, resourceId,
                                               annunNum, &unackAnnouncementSet));
    if (status.isOkay()) {
        status.add(TRACE, addSeverityAnnouncements(sessionId, resourceId,
                                                   annunNum, &ackAnnouncementSet));
        if (status.isOkay()) {
            status.add(TRACE, ackAnnouncements(sessionId, resourceId,
                                               annunNum, &ackAnnouncementSet));
        }
    }

    return status;
}

/*****************************************************************************
 * Delete the test announcements that were added by addTestAnnouncements().
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::deleteTestAnnouncements(SaHpiSessionIdT       sessionId,
                                                           SaHpiResourceIdT      resourceId,
                                                           SaHpiAnnunciatorNumT  annunNum) {
    HpiTestStatus  status;

    status.add(TRACE, deleteAnnouncements(sessionId, resourceId,
                                          annunNum, &unackAnnouncementSet));

    status.add(TRACE, deleteAnnouncements(sessionId, resourceId,
                                          annunNum, &ackAnnouncementSet));

    return status;
}

/*****************************************************************************
 * Add one announcement for each valid severity.
 *
 * WARNING: Be careful when using this function along with deleteAnnouncements().
 *          Even if an error is returned, one or more announcements could still
 *          be added, and thus need to be deleted.  Structure the code as follows:
 *
 *                status = addSeverityAnnouncements(..);
 *                if (status != SA_OK) {
 *                        .
 *                } else {
 *                        .
 *                }
 *                deleteAnnouncements(..);
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::addSeverityAnnouncements(SaHpiSessionIdT       sessionId,
                                                            SaHpiResourceIdT      resourceId,
                                                            SaHpiAnnunciatorNumT  annunNum,
                                                            AnnouncementSet       *announcementSet) {
    HpiTestStatus      status;
    int                severityCount;
    SaHpiSeverityT     *severity;
    SaHpiAnnouncementT announcement;

    severity = getValidSeverities(&severityCount);

    for (int i = 0; i < severityCount; i++) {

        AnnunciatorHelper::fill(&announcement, severity[i]);
        SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId,
                                             annunNum, &announcement);
        if (error == SA_OK) {
            announcementSet->add(announcement);
        } else {
            status.assertError(TRACE, ANNUNCIATOR_ADD, SA_OK, error, 
                               "Severity is %s.",
                               HpiString::severity(severity[i]));
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * Delete all of the announcements in the given set.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::deleteAnnouncements(SaHpiSessionIdT       sessionId,
                                                       SaHpiResourceIdT      resourceId,
                                                       SaHpiAnnunciatorNumT  annunNum,
                                                       AnnouncementSet       *announcementSet) {
    HpiTestStatus status;

    for (int i = 0; i < announcementSet->getCount(); i++) {
        SaHpiAnnouncementT *announcement = announcementSet->get(i);
        SaErrorT error = saHpiAnnunciatorDelete(sessionId, resourceId, annunNum, 
                                                announcement->EntryId, SAHPI_INFORMATIONAL);
        if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_DELETE, SA_OK, error);
        }
    }

    return status;
}

/*****************************************************************************
 * Acknowledge all of the announcements in the given set.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::ackAnnouncements(SaHpiSessionIdT       sessionId,
                                                    SaHpiResourceIdT      resourceId,
                                                    SaHpiAnnunciatorNumT  annunNum,
                                                    AnnouncementSet       *announcementSet) {
    HpiTestStatus status;

    for (int i = 0; i < announcementSet->getCount(); i++) {
        SaHpiAnnouncementT *announcement = announcementSet->get(i);
        SaErrorT error = saHpiAnnunciatorAcknowledge(sessionId, resourceId, annunNum,
                                                     announcement->EntryId,
                                                     announcement->Severity);
        if (error != SA_OK) {
            status.assertError(TRACE, ANNUNCIATOR_ACKNOWLEDGE, SA_OK, error);
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * Return the valid severities.
 *****************************************************************************/

SaHpiSeverityT *AnnunciatorTestCase::getValidSeverities(int *count) {
    static SaHpiSeverityT severity[] = { SAHPI_OK, SAHPI_MINOR,
                                         SAHPI_MAJOR, SAHPI_CRITICAL,
                                         SAHPI_INFORMATIONAL, SAHPI_DEBUG };

    *count = MAX_SEVERITIES;
    return severity;
}

/*****************************************************************************
 * Return the number of announcements in the Annunciator table with the
 * given severity and acknowledgement.  Note the ANNUN_ERROR is returned
 * if an announcement is retrieved that doesn't match the given parameters.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::getAnnouncementCount(SaHpiSessionIdT sessionId,
                                                        SaHpiResourceIdT resourceId,
                                                        SaHpiAnnunciatorNumT annunNum,
                                                        SaHpiSeverityT severity,
                                                        SaHpiBoolT unacknowledgedOnly,
                                                        int *count) {
    HpiTestStatus status;
    SaHpiAnnouncementT    announcement;

    *count = 0;
    announcement.EntryId = SAHPI_FIRST_ENTRY;
    while (status.isOkay()) {
        SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, annunNum, severity,
                                                 unacknowledgedOnly, &announcement);
        if (error == SA_ERR_HPI_NOT_PRESENT) {
            break;
        } else if (error != SA_OK) {
            status.assertFailure(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, error);
        } else {
            if (!hasSeverity(&announcement, severity)) {
                status.assertFailure(TRACE, 
                            "Retrieved announcement with non-matching severity!\n"
                            "Searching for %s; found %s.", HpiString::severity(severity),
                            HpiString::severity(announcement.Severity));
            } else if (unacknowledgedOnly && announcement.Acknowledged) {
                status.assertFailure(TRACE, 
                            "Retrieved an acknowledged announcement when "
                            "only unacknowledged were requested.");
            } else {
                (*count)++;
            }
        }
    }

    return status;
}

/*****************************************************************************
 * Does the given announcement have the given severity.  A severity
 * of SAHPI_ALL_SEVERITIES matches all severities.
 *****************************************************************************/

bool AnnunciatorTestCase::hasSeverity(SaHpiAnnouncementT  *announcement,
                                      SaHpiSeverityT severity) {
    if (severity == SAHPI_ALL_SEVERITIES) {
        return true;
    } else {
        return (announcement->Severity == severity);
    }
}

/*****************************************************************************
 * Find a severity level that is NOT being used by any of the announcements
 * in Annunciator table.  If all of the severity levels are being used, then
 * use INFORMATIONAL by default.  The "found" argument indicates whether
 * an unused severity was found or not.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::getUnusedSeverity(SaHpiSessionIdT        sessionId,
                                                     SaHpiResourceIdT       resourceId,
                                                     SaHpiAnnunciatorNumT   annunNum,
                                                     SaHpiBoolT             unacknowledgedOnly,
                                                     SaHpiSeverityT         *severity,
                                                     bool                   *found) {
    HpiTestStatus status;
    int        count, severityCount;

    *found = false;
    *severity = SAHPI_INFORMATIONAL;

    SaHpiSeverityT *severities = getValidSeverities(&severityCount);
    for (int i = 0; i < severityCount && status.isOkay(); i++) {
        status.add(TRACE, getAnnouncementCount(sessionId, resourceId,
                                               annunNum, severities[i],
                                               unacknowledgedOnly, &count));
        if (status.isOkay() && (count == 0)) {
            *found = true;
            *severity = severities[i];
            break;
        }
    }

    return status;
}

/*****************************************************************************
 * Add an announcement with the given severity. 
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::addAnnouncement(SaHpiSessionIdT        sessionId,
                                                   SaHpiResourceIdT       resourceId,
                                                   SaHpiAnnunciatorNumT   annunNum,
                                                   SaHpiSeverityT         severity,
                                                   SaHpiAnnouncementT     *announcement) {
    HpiTestStatus status;

    AnnunciatorHelper::fill(announcement, severity);
    SaErrorT error = saHpiAnnunciatorAdd(sessionId, resourceId, annunNum, announcement);
    if (error == SA_ERR_HPI_OUT_OF_SPACE) {
        status.assertNotSupport();
    } else if (error != SA_OK) {
        status.assertError(TRACE, ANNUNCIATOR_ADD, SA_OK, error);
    }

    return status;
}

/*****************************************************************************
 * Add an announcement with a severity of INFORMATIONAL.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::addInfoAnnouncement(SaHpiSessionIdT        sessionId,
                                                       SaHpiResourceIdT       resourceId,
                                                       SaHpiAnnunciatorNumT   annunNum,
                                                       SaHpiAnnouncementT     *announcement) {
    HpiTestStatus status;

    status.add(TRACE, addAnnouncement(sessionId, resourceId, annunNum,
                                      SAHPI_INFORMATIONAL, announcement));

    return status;
}

/*****************************************************************************
 * Delete the given annoucement.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::deleteAnnouncement(SaHpiSessionIdT        sessionId,
                                                      SaHpiResourceIdT       resourceId,
                                                      SaHpiAnnunciatorNumT   annunNum,
                                                      SaHpiAnnouncementT     *announcement) {
    HpiTestStatus status;

    SaErrorT error = saHpiAnnunciatorDelete(sessionId, resourceId, annunNum, 
                                            announcement->EntryId, SAHPI_ALL_SEVERITIES);
    status.checkError(TRACE, ANNUNCIATOR_DELETE, error);

    return status;
}

/*****************************************************************************
 * Get the set of severities that are not being used by any of the 
 * announcements in the Annunciator Table.
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::getUnusedSeverities(SaHpiSessionIdT        sessionId,
                                                       SaHpiResourceIdT       resourceId,
                                                       SaHpiAnnunciatorNumT   a_num,
                                                       SaHpiBoolT             unacknowledgedOnly,
                                                       SaHpiSeverityT         severity[],
                                                       int                    *scount) {
    HpiTestStatus   status;
    int          count;
    int          severityCount;

    *scount = 0;
    SaHpiSeverityT *severities = getValidSeverities(&severityCount);

    for (int i = 0; i < severityCount; i++) {
        status.add(TRACE, getAnnouncementCount(sessionId, resourceId, a_num, severities[i],
                                               unacknowledgedOnly, &count));
        if (!status.isOkay()) {
            break;
        } else if (count == 0) {
            severity[*scount] = severities[i];
            (*scount)++;
        }
    }

    return status;
}

/*****************************************************************************
 * Does the Annunciator Table contain an announcement with the given "entryId?"
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::containsAnnouncement(SaHpiSessionIdT        sessionId,
                                                        SaHpiResourceIdT       resourceId,
                                                        SaHpiAnnunciatorNumT   annunNum,
                                                        SaHpiEntryIdT          entryId,
                                                        bool                   *found) {
    HpiTestStatus status;
    SaHpiAnnouncementT  announcement;

    SaErrorT error = saHpiAnnunciatorGet(sessionId, resourceId, annunNum, entryId, &announcement);
    if (error == SA_OK) {
        *found = true;
    } else if (error == SA_ERR_HPI_NOT_PRESENT) {
        *found = false;
    } else {
        status.assertError(TRACE, ANNUNCIATOR_GET, SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}

/*****************************************************************************
 * Is the Annunciator Table empty? 
 *****************************************************************************/

HpiTestStatus AnnunciatorTestCase::isEmpty(SaHpiSessionIdT        sessionId,
                                           SaHpiResourceIdT       resourceId,
                                           SaHpiAnnunciatorNumT   a_num,
                                           bool                   *empty) {
    HpiTestStatus          status;
    SaHpiAnnouncementT  announcement;

    announcement.EntryId = SAHPI_FIRST_ENTRY;
    SaErrorT error = saHpiAnnunciatorGetNext(sessionId, resourceId, a_num,
                                             SAHPI_ALL_SEVERITIES, SAHPI_FALSE,
                                             &announcement);
    if (error == SA_OK) {
        *empty = false;
    } else if (error == SA_ERR_HPI_NOT_PRESENT) {
        *empty = true;
    } else {
        status.assertError(TRACE, ANNUNCIATOR_GET_NEXT, SA_OK, SA_ERR_HPI_NOT_PRESENT, error);
    }

    return status;
}


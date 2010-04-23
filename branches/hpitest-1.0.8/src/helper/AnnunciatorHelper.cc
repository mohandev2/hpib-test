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

#include "AnnunciatorHelper.h"
#include "HpiString.h"
#include "Report.h"
#include "HpiHelper.h"
#include "ConditionHelper.h"

/*****************************************************************************
 * Is the Annunciator Record valid?
 *****************************************************************************/

bool AnnunciatorHelper::isValid(SaHpiAnnunciatorRecT *rec, Report &report) {
    bool valid = true;

    if ((rec->AnnunciatorType < SAHPI_ANNUNCIATOR_TYPE_LED) ||
        (rec->AnnunciatorType > SAHPI_ANNUNCIATOR_TYPE_OEM)) {

        valid = false;
        report.add("Annunciator Type is invalid [0x%X].", rec->AnnunciatorType);
    }

    return valid;
}

/*****************************************************************************
 * Are the two Annunciator records the same or not?
 *****************************************************************************/

bool AnnunciatorHelper::isEqual(SaHpiAnnunciatorRecT *rec1, SaHpiAnnunciatorRecT *rec2) {
    Report report;
    return isEqual(rec1, rec2, report);
}

/*****************************************************************************
 * Are the two Annunciator records the same or not?  Also return a report
 * of what was not the same.
 *****************************************************************************/

bool AnnunciatorHelper::isEqual(SaHpiAnnunciatorRecT *rec1, 
                                SaHpiAnnunciatorRecT *rec2, Report &report) {
    bool equal = true;

    if (rec1->AnnunciatorNum != rec2->AnnunciatorNum) {
        equal = false;
        report.add("AnnunciatorNum fields are not equal [0x%X, 0x%X].", 
                   rec1->AnnunciatorNum, rec2->AnnunciatorNum);
    }

    if (rec1->AnnunciatorType != rec2->AnnunciatorType) {
        equal = false;
        report.add("AnnunciatorType fields are not equal [%s, %s].", 
                   HpiString::annunciatorType(rec1->AnnunciatorType), 
                   HpiString::annunciatorType(rec2->AnnunciatorType));
    }

    if (!HpiHelper::isBoolEqual(rec1->ModeReadOnly, rec2->ModeReadOnly)) {
        equal = false;
        report.add("Annunciator->ModeReadOnly fields are not equal [%s, %s].", 
                   HpiString::boolean(rec1->ModeReadOnly),
                   HpiString::boolean(rec2->ModeReadOnly));
    }

    if (rec1->MaxConditions != rec2->MaxConditions) {
        equal = false;
        report.add("Annunciator->MaxConditions fields are not equal [%d, %d].", 
                   rec1->MaxConditions, rec2->MaxConditions);
    }

    if (rec1->Oem != rec2->Oem) {
        equal = false;
        report.add("Annunciator->Oem fields are not equal [%d, %d].", 
                   rec1->Oem, rec2->Oem);
    }

    return equal;
}

/*****************************************************************************
 * Are the two Announcements the same or not?
 *****************************************************************************/

bool AnnunciatorHelper::isEqual(SaHpiAnnouncementT *a1,
                                SaHpiAnnouncementT *a2) {
    Report report;
    return isEqual(a1, a2, report);
}

/*****************************************************************************
 * Are the two Announcements the same or not?  Include a report of what
 * was not the same.
 *****************************************************************************/

bool AnnunciatorHelper::isEqual(SaHpiAnnouncementT *a1,
                                SaHpiAnnouncementT *a2, Report &report) {
    bool equal = true;

    if (a1->EntryId != a2->EntryId) {
        equal = false;
        report.add("Announcement->EntryId fields are not equal [0x%X, 0x%X].", 
                   a1->EntryId, a2->EntryId);
    }

    if (a1->Timestamp != a2->Timestamp) {
        equal = false;
        report.add("Announcement->Timestamp fields are not equal [%d, %d].", 
                   a1->Timestamp, a1->Timestamp);
    }

    if (!HpiHelper::isBoolEqual(a1->AddedByUser, a2->AddedByUser)) {
        equal = false;
        report.add("Announcement->AddedByUser fields are not equal [%s, %s].", 
                   HpiString::boolean(a1->AddedByUser), HpiString::boolean(a2->AddedByUser));
    }

    if (a1->Severity != a2->Severity) {
        equal = false;
        report.add("Announcement->Severity fields are not equal [%s, %s].", 
                   HpiString::severity(a1->Severity), HpiString::severity(a2->Severity));
    }

    if (!HpiHelper::isBoolEqual(a1->Acknowledged, a2->Acknowledged)) {
        equal = false;
        report.add("Announcement->Acknowledged fields are not equal [%s, %s].", 
                   HpiString::boolean(a1->Acknowledged), HpiString::boolean(a2->Acknowledged));
    }

    if (!ConditionHelper::isEqual(&a1->StatusCond, &a2->StatusCond, report)) {
        equal = false;
        report.add("Announcement->StatusCond fields are not equal.");
    }

    return equal;
}

/*****************************************************************************
 * Fill an announcement with a severity of SAHPI_INFORMATIONAL.  Useful for
 * creating an announcement to add to the Annunciator Table.
 *****************************************************************************/

void AnnunciatorHelper::fill(SaHpiAnnouncementT *announcement) {
    fill(announcement, SAHPI_INFORMATIONAL);
}

/*****************************************************************************
 * Fill an announcement with a given severity.  Useful for creating an 
 * announcement to add to the Annunciator Table.
 *****************************************************************************/

void AnnunciatorHelper::fill(SaHpiAnnouncementT *announcement, SaHpiSeverityT severity) {
    announcement->EntryId = 0;
    announcement->Timestamp = SAHPI_TIME_UNSPECIFIED;
    announcement->AddedByUser = SAHPI_TRUE;
    announcement->Severity = severity;
    announcement->Acknowledged = SAHPI_FALSE;
    ConditionHelper::fill(&announcement->StatusCond);
}

/*****************************************************************************
 * Is the given Annunciator Mode one of the valid enumerated values or not?
 *****************************************************************************/

bool AnnunciatorHelper::isValidMode(SaHpiAnnunciatorModeT mode) {
    return ((mode == SAHPI_ANNUNCIATOR_MODE_AUTO) ||
            (mode == SAHPI_ANNUNCIATOR_MODE_USER) ||
            (mode == SAHPI_ANNUNCIATOR_MODE_SHARED));
}

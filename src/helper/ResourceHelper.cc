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
 *
 * Changes:
 * 09/06/24 saxena.anurag@emerson.com
 *          Add getInvalidEntityPath
 * 09/08/06 lars.wetzel@emerson.com
 *          Small chg of getInvalidEntityPath
 */

#include <cstring>
#include "ResourceHelper.h"
#include "Report.h"
#include "TextBufferHelper.h"
#include "HpiString.h"
#include "HpiHelper.h"

/*****************************************************************************
 * Fill a entity path with a default value.
 *****************************************************************************/

void ResourceHelper::fillEntityPath(SaHpiEntityPathT *entityPath) {
    entityPath->Entry[0].EntityType = SAHPI_ENT_ROOT;
    entityPath->Entry[0].EntityLocation = 0;
}

/*****************************************************************************
 * Fill a entity path with an invalid value.
 *****************************************************************************/

void ResourceHelper::getInvalidEntityPath(SaHpiEntityPathT *entityPath) {
    entityPath->Entry[0].EntityType = SAHPI_ENT_AMC;
    entityPath->Entry[0].EntityLocation = 1;
    entityPath->Entry[1].EntityType = SAHPI_ENT_ROOT;
    entityPath->Entry[1].EntityLocation = 0;
}

/*****************************************************************************
 * Are the two RPT entries the same or not?
 *****************************************************************************/

bool ResourceHelper::isEqual(SaHpiRptEntryT *rptEntry1, SaHpiRptEntryT *rptEntry2) {
    Report report;
    return isEqual(rptEntry1, rptEntry2, report);
}

/*****************************************************************************
 * Are the two RPT entries the same or not?
 *****************************************************************************/

bool ResourceHelper::isEqual(SaHpiRptEntryT *rptEntry1, SaHpiRptEntryT *rptEntry2, Report &report) {
    bool equal = true;

    if (rptEntry1->EntryId != rptEntry2->EntryId) {
        equal = false;
        report.add("RptEntry->EntryId fields are not equal [0x%X, 0x%X].", 
                   rptEntry1->EntryId, rptEntry2->EntryId);
    } 
    
    if (rptEntry1->ResourceId != rptEntry2->ResourceId) {
        equal = false;
        report.add("RptEntry->ResourceId fields are not equal [0x%X, 0x%X].", 
                   rptEntry1->ResourceId, rptEntry2->ResourceId);
    } 
    
    if (!isEqual(&rptEntry1->ResourceInfo, &rptEntry2->ResourceInfo, report)) {
        equal = false;
    } 
    
    if (!isEqual(&rptEntry1->ResourceEntity, &rptEntry2->ResourceEntity)) {
        equal = false;
        report.add("RptEntry->ResourceEntity fields are not equal [%s, %s].", 
                   HpiString::entityPath(&rptEntry1->ResourceEntity),
                   HpiString::entityPath(&rptEntry2->ResourceEntity));
    } 
    
    if (rptEntry1->ResourceCapabilities != rptEntry2->ResourceCapabilities) {
        equal = false;
        report.add("RptEntry->ResourceCapabilities fields are not equal [0x%X, 0x%X].", 
                   rptEntry1->ResourceCapabilities, rptEntry2->ResourceCapabilities);
    } 
    
    if (rptEntry1->HotSwapCapabilities != rptEntry2->HotSwapCapabilities) {
        equal = false;
        report.add("RptEntry->HotSwapCapabilities fields are not equal [0x%X, 0x%X].", 
                   rptEntry1->HotSwapCapabilities, rptEntry2->HotSwapCapabilities);
    } 
    
    if (rptEntry1->ResourceSeverity != rptEntry2->ResourceSeverity) {
        equal = false;
        report.add("RptEntry->ResourceSeverity fields are not equal [%s, %s].", 
                   HpiString::severity(rptEntry1->ResourceSeverity), 
                   HpiString::severity(rptEntry2->ResourceSeverity));
    } 
    
    if (!HpiHelper::isBoolEqual(rptEntry1->ResourceFailed, rptEntry2->ResourceFailed)) {
        equal = false;
        report.add("RptEntry->ResourceFailed fields are not equal [%s, %s].", 
                   HpiString::boolean(rptEntry1->ResourceFailed), 
                   HpiString::boolean(rptEntry2->ResourceFailed));
    } 
    
    if (!TextBufferHelper::isEqual(&rptEntry1->ResourceTag, &rptEntry2->ResourceTag, report)) {
        equal = false;
    }

    return equal;
}

/*****************************************************************************
 * Are the two Resource Infos the same or not?
 *****************************************************************************/

bool ResourceHelper::isEqual(SaHpiResourceInfoT *info1, SaHpiResourceInfoT *info2, Report &report) {
    bool equal = true;

    if (info1->ResourceRev != info2->ResourceRev) {
        equal = false;
        report.add("ResourceInfo->ResourceRev fields are not equal [0x%X, 0x%X].", 
                   info1->ResourceRev, info2->ResourceRev);
    } 
    
    if (info1->SpecificVer != info2->SpecificVer) {
        equal = false;
        report.add("ResourceInfo->SpecificVer fields are not equal [0x%X, 0x%X].", 
                   info1->SpecificVer, info2->SpecificVer);
    } 
    
    if (info1->DeviceSupport != info2->DeviceSupport) {
        equal = false;
        report.add("ResourceInfo->DeviceSupport fields are not equal [0x%X, 0x%X].", 
                   info1->DeviceSupport, info2->DeviceSupport);
    } 
    
    if (info1->ManufacturerId != info2->ManufacturerId) {
        equal = false;
        report.add("ResourceInfo->ManufacturerId fields are not equal [0x%X, 0x%X].", 
                   info1->ManufacturerId, info2->ManufacturerId);
    } 
    
    if (info1->ProductId != info2->ProductId) {
        equal = false;
        report.add("ResourceInfo->ProductId fields are not equal [0x%X, 0x%X].", 
                   info1->ProductId, info2->ProductId);
    } 
    
    if (info1->FirmwareMajorRev != info2->FirmwareMajorRev) {
        equal = false;
        report.add("ResourceInfo->FirmwareMajorRev fields are not equal [0x%X, 0x%X].", 
                   info1->FirmwareMajorRev, info2->FirmwareMajorRev);
    } 
    
    if (info1->FirmwareMinorRev != info2->FirmwareMinorRev) {
        equal = false;
        report.add("ResourceInfo->FirmwareMinorRev fields are not equal [0x%X, 0x%X].", 
                   info1->FirmwareMinorRev, info2->FirmwareMinorRev);
    } 

    if (info1->AuxFirmwareRev != info2->AuxFirmwareRev) {
        equal = false;
        report.add("ResourceInfo->AuxFirmwareRev fields are not equal [0x%X, 0x%X].", 
                   info1->AuxFirmwareRev, info2->AuxFirmwareRev);
    } 

    if (memcmp(info1->Guid, info2->Guid, sizeof(info1->Guid))) {
        equal = false;
        report.add("ResourceInfo->Guid fields are not equal.");
    }

    return equal;
}

/*****************************************************************************
 * Are the two Entity Paths the same or not?
 *****************************************************************************/

bool ResourceHelper::isEqual(SaHpiEntityPathT *path1, SaHpiEntityPathT *path2) {
    for (int i = 0; i < SAHPI_MAX_ENTITY_PATH; i++) {
        if (path1->Entry[i].EntityType != path2->Entry[i].EntityType) {
            return false;
        } else if (path1->Entry[i].EntityLocation != path2->Entry[i].EntityLocation) {
            return false;
        }

        if (path1->Entry[i].EntityType == SAHPI_ENT_ROOT) {
            break;
        }
    }
    return true;
}

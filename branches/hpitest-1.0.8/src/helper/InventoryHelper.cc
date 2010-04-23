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

#include "InventoryHelper.h"
#include "Report.h"
#include "HpiHelper.h"
#include "TextBufferHelper.h"
#include "HpiString.h"

/*****************************************************************************
 * Is the Inventory Record valid or not?  NOTE: There is nothing to check.
 * It is always valid.
 *****************************************************************************/

bool InventoryHelper::isValid(SaHpiInventoryRecT *, Report &) {
    return true;
}

/*****************************************************************************
 * Are the two Inventory Records the same or not?
 *****************************************************************************/

bool InventoryHelper::isEqual(SaHpiInventoryRecT *rec1, SaHpiInventoryRecT *rec2) {
    Report report;
    return isEqual(rec1, rec2, report);
}

/*****************************************************************************
 * Are the two Inventory Records the same or not?  Include a report of the
 * differences.
 *****************************************************************************/

bool InventoryHelper::isEqual(SaHpiInventoryRecT *rec1, SaHpiInventoryRecT *rec2, Report &report) {
    bool equal = true;

    if (rec1->IdrId != rec2->IdrId) {
        equal = false;
        report.add("InventoryRec->IdrId fields are not equal [0x%X, 0x%X].", rec1->IdrId, rec2->IdrId);
    }

    if (!HpiHelper::isBoolEqual(rec1->Persistent, rec2->Persistent)) {
        equal = false;
        report.add("InventoryRec->Persistent fields are not equal [%s, %s].", 
                    HpiString::boolean(rec1->Persistent), HpiString::boolean(rec2->Persistent));
    }

    if (rec1->Oem != rec2->Oem) {
        equal = false;
        report.add("InventoryRec->Oem fields are not equal [0x%X, 0x%X].", rec1->Oem, rec2->Oem);
    }

    return equal;
}

/*****************************************************************************
 * Fill an IDR Field with a specific Area Id and a Field Id of zero.
 *****************************************************************************/

void InventoryHelper::fill(SaHpiIdrFieldT *field, SaHpiEntryIdT areaId) {
    fill(field, areaId, 0);
}

/*****************************************************************************
 * Fill an IDR Field with a specific Area Id and Field Id.
 *****************************************************************************/

void InventoryHelper::fill(SaHpiIdrFieldT *field, SaHpiEntryIdT areaId, SaHpiEntryIdT fieldId) {
    field->AreaId = areaId;
    field->FieldId = fieldId;
    field->Type = SAHPI_IDR_FIELDTYPE_CUSTOM;
    field->ReadOnly = SAHPI_FALSE;
    TextBufferHelper::fill(&field->Field);
}


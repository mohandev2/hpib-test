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
 * 2009/07/30 lars.wetzel@emerson.com
 *            Add IDR Field comparison
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
 * Are the two Inventory Fields the same or not?
 *****************************************************************************/
bool InventoryHelper::isEqual(SaHpiIdrFieldT *field1, SaHpiIdrFieldT *field2) {
  Report report;
  return isEqual(field1, field2, report);
}

/*****************************************************************************
 * Are the two Inventory Fields the same or not?  Include a report of the
 * differences.
 *****************************************************************************/
bool InventoryHelper::isEqual(SaHpiIdrFieldT *field1, SaHpiIdrFieldT *field2, 
			      Report &report) {
  bool equal = true;

  if (field1->AreaId != field2->AreaId) {
    equal = false;
    report.add("IdrField->AreaId fields are not equal [0x%X, 0x%X].", field1->AreaId, field2->AreaId);
  }

  if (field1->FieldId != field2->FieldId) {
    equal = false;
    report.add("IdrField->FieldId fields are not equal [0x%X, 0x%X].", field1->FieldId, field2->FieldId);
  }

  if (field1->Type != field2->Type) {
    equal = false;
    report.add("IdrField->Type fields are not equal [%s, %s].", 
	       HpiString::idrFieldType(field1->Type), HpiString::idrFieldType(field2->Type));
  }

  if (!HpiHelper::isBoolEqual(field1->ReadOnly, field2->ReadOnly)) {
    equal = false;
    report.add("IdrField->ReadOnly Attribute are not equal [%s, %s].",
	       HpiString::boolean(field1->ReadOnly),
	       HpiString::boolean(field2->ReadOnly));
  }

  if (!TextBufferHelper::isEqual(&field1->Field, &field2->Field, report)) {
    equal = false;
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


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

#ifndef __INVENTORYHELPER_H__
#define __INVENTORYHELPER_H__

#include "SafTest.h"

class Report;

/*****************************************************************************
 * Inventory Helper
 *****************************************************************************/

class InventoryHelper
{
public:
    static bool isValid(SaHpiInventoryRecT *rec, Report &report);

    static bool isEqual(SaHpiInventoryRecT *rec1, SaHpiInventoryRecT *rec2);
    static bool isEqual(SaHpiInventoryRecT *rec1, SaHpiInventoryRecT *rec2, Report &report);
    
    static bool isEqual(SaHpiIdrFieldT *field1, SaHpiIdrFieldT *field2);
    static bool isEqual(SaHpiIdrFieldT *field1, SaHpiIdrFieldT *field2, Report &report);

    static void fill(SaHpiIdrFieldT *field, SaHpiEntryIdT areaId);
    static void fill(SaHpiIdrFieldT *field, SaHpiEntryIdT areaId, SaHpiEntryIdT fieldId);
};

#endif


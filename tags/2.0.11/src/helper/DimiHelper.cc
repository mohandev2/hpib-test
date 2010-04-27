/*
 * (C) Copyright Pigeon Point Systems, 2009
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
 *     Anton Pak <anton.pak@pigeonpoint.com>
 */

#include "DimiHelper.h"
#include "Report.h"

/*****************************************************************************
 * Is the Dimi Record valid?
 *****************************************************************************/

bool DimiHelper::isValid(SaHpiDimiRecT *, Report &) {
    return true;
}

/*****************************************************************************
 * Are the two Dimi records the same or not?
 *****************************************************************************/

bool DimiHelper::isEqual(SaHpiDimiRecT *rec1, SaHpiDimiRecT *rec2) {
    Report report;
    return isEqual(rec1, rec2, report);
}

/*****************************************************************************
 * Are the two Dimi records the same or not?  Also return a report
 * of what was not the same.
 *****************************************************************************/

bool DimiHelper::isEqual(SaHpiDimiRecT *rec1, SaHpiDimiRecT *rec2, Report &report) {
    bool equal = true;

    if (rec1->DimiNum != rec2->DimiNum) {
        equal = false;
        report.add("DimiNum fields are not equal [0x%X, 0x%X].", 
                   rec1->DimiNum, rec2->DimiNum);
    }

    if (rec1->Oem != rec2->Oem) {
        equal = false;
        report.add("Dimi->Oem fields are not equal [%d, %d].", 
                   rec1->Oem, rec2->Oem);
    }

    return equal;
}


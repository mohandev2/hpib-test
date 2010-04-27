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

#include "NameHelper.h"

/*****************************************************************************
 * Fill with default value.
 *****************************************************************************/

void NameHelper::fill(SaHpiNameT *name) {
    name->Length = 4;
    name->Value[0] = 'T';
    name->Value[1] = 'e';
    name->Value[2] = 's';
    name->Value[3] = 't';
}

/*****************************************************************************
 * Are the two names equal or not?
 *****************************************************************************/

bool NameHelper::isEqual(SaHpiNameT *name1, SaHpiNameT *name2) {
    if (name1->Length != name2->Length) {
        return false;
    } else {
        for (int i = 0; i < name1->Length; i++) {
            if (name1->Value[i] != name2->Value[i]) {
                return false;
            }
        }
    }

    return true;
}


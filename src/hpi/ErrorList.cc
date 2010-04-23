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

#include "ErrorList.h"

/*****************************************************************************
 * Constructor.
 *****************************************************************************/

ErrorList::ErrorList() { 
    count = 0; 
}

/*****************************************************************************
 * Add an error to the list.
 *****************************************************************************/

void ErrorList::add(SaErrorT error) {
    if (count < MAX_ERRORS) {
        errorList[count] = error;
        count++;
    }
}

/*****************************************************************************
 * Return the number of errors in the list.
 *****************************************************************************/

int ErrorList::getCount() {
    return count;
}

/*****************************************************************************
 * Return an error from the list.
 *****************************************************************************/

SaErrorT ErrorList::get(int index) {
    return errorList[index];
}

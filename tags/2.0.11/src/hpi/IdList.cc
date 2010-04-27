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

#include <cstdio>
#include <iostream>
#include "IdList.h"

/*****************************************************************************
 * Constructor
 *****************************************************************************/

IdList::IdList() {
    count = 0;
    size = 0;
    id = NULL;
}

/*****************************************************************************
 * Destructor
 *****************************************************************************/

IdList::~IdList() {
    if (id != NULL) {
        delete [] id;
    }
}

/*****************************************************************************
 * Add an ID to the list.  Return false if the ID is already on the list;
 * otherwise return true to indicate that the ID was added.
 *
 * If there isn't space available in the array, allocate a larger array and
 * copy the current IDs to that new array.
 *****************************************************************************/

bool IdList::add(SaHpiUint32T new_id) {
    if (contains(new_id)) {
        return false;
    } else {
        if (count == size) {
            size += INCREMENT;
            SaHpiUint32T *old_id = id;
            id = new SaHpiUint32T[size];
            if (id == NULL) {
                std::cerr << "Memory allocation of id array failed." << std::endl;
                size -= INCREMENT;
                id = old_id;
            } else if (old_id != NULL) {
                for (int i = 0; i < count; i++) {
                    id[i] = old_id[i];
                }
                delete [] old_id;
            }
        }

        id[count] = new_id;
        count++;

        return true;
    }
}

/*****************************************************************************
 * Return the number of IDs.
 *****************************************************************************/

int IdList::getCount() {
    return count;
}

/*****************************************************************************
 * Return a specific ID.
 *****************************************************************************/

SaHpiUint32T IdList::getId(int index) {
    return id[index];
}

/*****************************************************************************
 * Clear the list of IDs.
 *****************************************************************************/

void IdList::clear() {
    count = 0;
    size = 0;
    if (id != NULL) {
        delete [] id;
    }
    id = NULL;
}

/*****************************************************************************
 * Is the given ID already contained in the list?
 *****************************************************************************/

bool IdList::contains(SaHpiUint32T searchId) {
    for (int i = 0; i < count; i++) {
        if (this->id[i] == searchId) {
            return true;
        }
    }
    return false;
}


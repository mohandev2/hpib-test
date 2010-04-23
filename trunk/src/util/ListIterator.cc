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
#include "ListIterator.h"

/*****************************************************************************
 * Default Constructor.  Store a pointer to the head of the List.
 *****************************************************************************/

ListIterator::ListIterator(ListItem *head) { 
    cur = head; 
}

/*****************************************************************************
 * Copy Constructor.
 *****************************************************************************/

ListIterator::ListIterator(const ListIterator &iterator) {
    cur = iterator.cur;
}

/*****************************************************************************
 * Assignment Operator.  
 *****************************************************************************/

ListIterator ListIterator::operator=(ListIterator iterator) {
    cur = iterator.cur;

    return *this;
}

/*****************************************************************************
 * Is there another object in the List?
 *****************************************************************************/

bool ListIterator::hasNext() {
    return (cur != NULL);
}

/*****************************************************************************
 * Return the next object in the List and advance
 * the pointer to the next item in the List.
 *****************************************************************************/

void *ListIterator::next() {
    void *object = cur->get();
    cur = cur->next;
    return object;
}
